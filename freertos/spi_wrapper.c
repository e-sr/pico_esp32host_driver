#include "esp_check.h"
#include "esp_log.h"

#include "os_wrapper.h"
#include "transport_drv.h"
#include "spi_wrapper.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "hardware/dma.h"

#define SPI_PORT spi0

static inline void cs_select()
{
    asm volatile("nop \n nop \n nop");
    gpio_put(PIN_SPI_CS, 0); // Active low
    asm volatile("nop \n nop \n nop");
}

static inline void cs_deselect()
{
    asm volatile("nop \n nop \n nop");
    gpio_put(PIN_SPI_CS, 1);
    asm volatile("nop \n nop \n nop");
}

DEFINE_LOG_TAG(spi_wrapper);

// mutex to lock SPI transaction
void *spi_trans_mutex = NULL;

#if SPI_DMA
// semaphore for SPI transaction complete
void *spi_trans_complete_sem = NULL;
// SPI transaction complete Callback Handler , attenzione forse solo necessario se la transaction é non bloccante, fatta con dma
void __isr_callbacksem(void *hspi)
{
    hosted_post_semaphore_from_isr(spi_trans_complete_sem);
}
// Grab some unused dma channels
uint dma_tx = 0;
uint dma_rx = 0;
#endif

void *hosted_spi_init(void)
{
    // Enable SPI at 1 MHz and connect to GPIOs
    spi_init(SPI0_BASE, H_SPI_INIT_CLK_MHZ);
    gpio_set_function(PIN_SPI_CLK, GPIO_FUNC_SPI);
    gpio_init(PIN_SPI_CS);
    gpio_set_function(PIN_SPI_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SPI_MOSI, GPIO_FUNC_SPI);
    spi_set_format(SPI_PORT, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

#if SPI_DMA
    dma_tx = dma_claim_unused_channel(true);
    dma_rx = dma_claim_unused_channel(true);
    spi_trans_complete_sem = hosted_create_semaphore(1);
    if (NULL == spi_trans_complete_sem)
    {
        ESP_LOGE(TAG, "failed to create SPI transaction semaphore");
        return NULL;
    }
    hosted_get_semaphore(spi_trans_complete_sem, HAL_MAX_DELAY);
#endif

    spi_trans_mutex = hosted_create_mutex();
    if (NULL == spi_trans_mutex)
    {
        ESP_LOGE(TAG, "failed to create SPI transaction mutex");
        return NULL;
    }
    return SPI_PORT;
}
/* Software controlled NSS */
#if !SPI_DMA
static inline stm_ret_t spi_transaction(uint8_t *txbuff, uint8_t *rxbuff, uint16_t size)
{
    uint32_t retval = 0;
    ESP_LOGV(TAG, "Execute SPI transaction\n");
    hosted_lock_mutex(spi_trans_mutex, portMAX_DELAY);
    retval = spi_write_blocking(SPI_PORT, txbuff, rxbuff, size);
    hosted_unlock_mutex(spi_trans_mutex);
    return retval;
}

#else

static inline stm_ret_t spi_transaction(uint8_t *txbuff, uint8_t *rxbuff, uint16_t size)
{
    HAL_StatusTypeDef retval = HAL_ERROR;
    hosted_lock_mutex(spi_trans_mutex, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(USR_SPI_CS_GPIO_Port, USR_SPI_CS_Pin, GPIO_PIN_RESET);
    memcpy(spi_tx_buffer, txbuff, size);
    SCB_CleanDCache_by_Addr(spi_tx_buffer, BUFFER_ALIGNED_SIZE);
    retval = HAL_SPI_TransmitReceive_DMA(&SPI_BUS_HAL, spi_tx_buffer, spi_rx_buffer, size);
    hosted_get_semaphore(spi_trans_complete_sem, HAL_MAX_DELAY);
    SCB_CleanInvalidateDCache_by_Addr(spi_rx_buffer, BUFFER_ALIGNED_SIZE);
    memcpy(rxbuff, spi_rx_buffer, size);
    HAL_GPIO_WritePin(USR_SPI_CS_GPIO_Port, USR_SPI_CS_Pin, GPIO_PIN_SET);
    hosted_unlock_mutex(spi_trans_mutex);

    return retval;
}
#endif

int hosted_do_spi_transfer(void *trans)
{
    struct hosted_transport_context_t *spi_trans = trans;
    assert(trans);

    return spi_transaction(spi_trans->tx_buf, spi_trans->rx_buf,
                           spi_trans->tx_buf_size);
}