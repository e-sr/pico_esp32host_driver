#ifndef __ESP_HOSTED_CONFIG_H__
#define __ESP_HOSTED_CONFIG_H__

#include <stdbool.h>

#include "esp_task.h"
#include "esp_err.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "pinconfig.h"
/* This file is to tune the main ESP-Hosted configurations.
 * In case you are not sure of some value, Let it be default.
 **/

#define H_TRANSPORT_NONE 0
#define H_TRANSPORT_SDIO 1
#define H_TRANSPORT_SPI_HD 2
#define H_TRANSPORT_SPI 3
#define H_TRANSPORT_UART 4

#define H_TRANSPORT_IN_USE H_TRANSPORT_SPI

/*  ========================== SPI Master Config start ======================  */
/*
Pins in use. The SPI Master can use the GPIO mux,
so feel free to change these if needed.
*/

enum
{
  SLAVE_CHIPSET_ESP32,
  SLAVE_CHIPSET_ESP32C2,
  SLAVE_CHIPSET_ESP32C3,
  SLAVE_CHIPSET_ESP32C6,
  SLAVE_CHIPSET_ESP32S2,
  SLAVE_CHIPSET_ESP32S3,
  SLAVE_CHIPSET_MAX_SUPPORTED
};

/* Select the slave chipset: only enable one */
// #define CONFIG_SLAVE_CHIPSET_ESP32 1
//  #define CONFIG_SLAVE_CHIPSET_ESP32C2 1
//  #define CONFIG_SLAVE_CHIPSET_ESP32C3 1
//  #define CONFIG_SLAVE_CHIPSET_ESP32C5 1
#define CONFIG_SLAVE_CHIPSET_ESP32C6 1
// #define CONFIG_SLAVE_CHIPSET_ESP32S2 1
// #define CONFIG_SLAVE_CHIPSET_ESP32S3 1

/* Log level to be set:
 * 5 - verbose
 * 4 - debug
 * 3 - info
 * 2 - warn
 * 1 - err
 *
 * Higher the level, higher the logs
 */
#define CONFIG_LOG_MAXIMUM_LEVEL 3

/* Mempool for spi transport allocations */
// #define H_USE_MEMPOOL 1
#define H_USE_MEMPOOL 0
/* Mmmory alignement for mempool blocks */
#define H_DMA_OR_CACHE_ALIGNMENT_BYTES 4

#define H_GPIO_HANDSHAKE_Pin PIN_HANDSHAKE
#define H_GPIO_HANDSHAKE_Port NULL

#define H_GPIO_DATA_READY_Port NULL
#define H_GPIO_DATA_READY_Pin PIN_DATA_READY

#define H_GPIO_RESET_Pin PIN_RESET
#define H_GPIO_RESET_Port NULL

#define USR_SPI_CS_Pin PIN_SPI_CS

#define GPIO_DATA_READY_Pin PIN_DATA_READY

#define GPIO_MOSI_Pin PIN_SPI_MOSI

#define GPIO_MISO_Pin PIN_SPI_MISO

#define GPIO_CLK_Pin PIN_SPI_CLK

//questi valori vanno definiti con i valori usatin nello slave
#define H_SPI_MODE                                   SPI_CPHA_0
#define H_SPI_INIT_CLK_MHZ                           5000000


// active
#ifdef H_RESET_ACTIVE_HIGH
#error "H_RESET_ACTIVE_HIGH is not supported"
#else
#define H_RESET_VAL_ACTIVE false
#define H_RESET_VAL_INACTIVE true
#endif

#define gpio_pin_state_t bool

#define H_GPIO_MODE_DEF_INPUT GPIO_IN
#define H_GPIO_MODE_DEF_OUTPUT GPIO_OUT

#if H_HANDSHAKE_ACTIVE_HIGH
#error "H_DATAREADY_ACTIVE_HIGH is not supported"
#else
#define H_HS_VAL_ACTIVE 0
#define H_HS_VAL_INACTIVE 1
#define H_HS_INTR_EDGE GPIO_IRQ_EDGE_FALL
#endif

#if H_DATAREADY_ACTIVE_HIGH
// error
#error "H_DATAREADY_ACTIVE_HIGH is not supported"
#else
#define H_DR_VAL_ACTIVE 0
#define H_DR_VAL_INACTIVE 1
#define H_DR_INTR_EDGE GPIO_IRQ_EDGE_FALL
#endif
/*  --------------------------- RPC Host Config start -----------------------  */
#define H_MAX_SIMULTANEOUS_SYNC_RPC_REQUESTS 5
#define H_MAX_SIMULTANEOUS_ASYNC_RPC_REQUESTS 5
#define H_DEFAULT_RPC_RSP_TIMEOUT 5
/*  --------------------------- RPC Host Config end -------------------------  */

/* At slave, if Wi-Fi tx is failing (maybe due to unstable wifi connection),
 * as preventive measure, host can start dropping the packets, depending upon slave load
 * Typically, low threshold is 60, high threshold = 90 for spi, 80 for sdio
 * 0 means disable the feature
 */
#define H_WIFI_TX_DATA_THROTTLE_LOW_THRESHOLD 0
#define H_WIFI_TX_DATA_THROTTLE_HIGH_THRESHOLD 0

/* Raw Throughput Testing */
#define H_TEST_RAW_TP 0
#define H_ESP_RAW_TP_REPORT_INTERVAL 10
#define H_ESP_RAW_TP_HOST_TO_ESP_PKT_LEN 1460

/* Raw throughput direction
 * Raw throughput is pure transport performance, without involving Wi-Fi etc layers
 * - For transport test from host to slave, enable CONFIG_ESP_RAW_THROUGHPUT_TX_TO_SLAVE
 * - For transport test from slave to host, enable CONFIG_ESP_RAW_THROUGHPUT_RX_FROM_SLAVE
 * - For bi-directional transport test, enable CONFIG_ESP_RAW_THROUGHPUT_BIDIRECTIONAL
 */
// #define CONFIG_ESP_RAW_THROUGHPUT_TX_TO_SLAVE        1
// #define CONFIG_ESP_RAW_THROUGHPUT_RX_FROM_SLAVE      1
#define CONFIG_ESP_RAW_THROUGHPUT_BIDIRECTIONAL 1

/* Use or don't use NVS on co-processor to save WiFi Config */
#if CONFIG_ESP_WIFI_NVS_ENABLED
#define H_ESP_WIFI_NVS_ENABLED 1
#else
#define H_ESP_WIFI_NVS_ENABLED 0
#endif

#if H_TEST_RAW_TP
#if CONFIG_ESP_RAW_THROUGHPUT_TX_TO_SLAVE
#define H_TEST_RAW_TP_DIR (ESP_TEST_RAW_TP__HOST_TO_ESP)
#elif CONFIG_ESP_RAW_THROUGHPUT_RX_FROM_SLAVE
#define H_TEST_RAW_TP_DIR (ESP_TEST_RAW_TP__ESP_TO_HOST)
#elif CONFIG_ESP_RAW_THROUGHPUT_BIDIRECTIONAL
#define H_TEST_RAW_TP_DIR (ESP_TEST_RAW_TP__BIDIRECTIONAL)
#else
#error Test Raw TP direction not defined
#endif
#else
#define H_TEST_RAW_TP_DIR (ESP_TEST_RAW_TP_NONE)
#endif

/*  ========================== SPI Master Config end ========================  */

esp_err_t esp_hosted_set_default_config(void);
bool esp_hosted_is_config_valid(void);

#define PRE_FORMAT_NEWLINE_CHAR "\r"
#define POST_FORMAT_NEWLINE_CHAR "\n"

#define CONFIG_H_LOWER_MEMCOPY 0

/* Do not use standard c malloc and hook to freertos heap4 */
#define USE_STD_C_LIB_MALLOC 1

#define H_MEM_STATS 0 // disable for now

#define H_ESP_PKT_STATS 1

/* netif & lwip or rest config included from sdkconfig */
#include "sdkconfig.h"

#endif /*__ESP_HOSTED_CONFIG_H__*/
