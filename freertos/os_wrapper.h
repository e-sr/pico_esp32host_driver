// Copyright 2015-2021 Espressif Systems (Shanghai) PTE LTD
/* SPDX-License-Identifier: GPL-2.0 OR Apache-2.0 */

#ifndef __OS_WRAPPER_H__
#define __OS_WRAPPER_H__

#include "os_header.h"
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>


#include "mempool.h"
#include "esp_hosted_config.h"

//#include "esp_timer.h"
//#include "esp_event.h"
//#include "netdev_if.h"
#include "esp_wifi_default.h"
#include "hosted_os_adapter.h"

/* port the character printing to uart */
#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
 set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

extern struct mempool * nw_mp_g;

//ESP_EVENT_DECLARE_BASE(WIFI_EVENT);
#define MCU_SYS                                      1

#include "common.h"


#define RPC__TIMER_ONESHOT                           0
#define RPC__TIMER_PERIODIC                          1

#define HOSTED_BLOCKING                              -1
#define HOSTED_NON_BLOCKING                          0

typedef struct {
	TaskHandle_t t_hl;
} thread_handle_t;

typedef struct {
	QueueHandle_t q_hl;
} queue_handle_t;

typedef struct {
	SemaphoreHandle_t s_hl;
} semaphore_handle_t;

typedef struct {
	SemaphoreHandle_t m_hl;
} mutex_handle_t;

#define spinlock_handle_t                            portMUX_TYPE

/* this is needed when there is no gpio port being used */
#define H_GPIO_PORT_DEFAULT                          -1


#define HOSTED_BLOCK_MAX                             portMAX_DELAY


#define RET_OK                                       0
#define RET_FAIL                                     -1
#define RET_INVALID                                  -2
#define RET_FAIL_MEM                                 -3
#define RET_FAIL4                                    -4
#define RET_FAIL_TIMEOUT                             -5


/* This is [malloc + aligned DMA] */
#define MEM_DMA_ALIGNMENT                4 /* 4 bytes */
#define MEM_ALLOC(x)                     MALLOC((x + (MEM_DMA_ALIGNMENT-1)) & (~(MEM_DMA_ALIGNMENT-1)))

/* without alignment */
#define MALLOC(x)                        pvPortMalloc(x)
#define FREE(x)                          vPortFree(x)


#define MAX_TRANSPORT_BUFFER_SIZE        MAX_SPI_BUFFER_SIZE

#define MAX_PAYLOAD_SIZE (MAX_SPI_BUFFER_SIZE-H_ESP_PAYLOAD_HEADER_OFFSET)


/* code segment attribute to load in a faster memory */
#define IRAM_ATTR


#define MILLISEC_TO_SEC			1000
#define TICKS_PER_SEC(x) (1000*(x) / portTICK_PERIOD_MS)
#define SEC_TO_MILLISEC(x) (1000*(x))
#define SEC_TO_MICROSEC(x) (1000*1000*(x))


#define MEM_DUMP(s)


#if 0
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 4, 0)
  #define ESP_MUTEX_INIT(mUtEx) portMUX_INITIALIZE(&(mUtEx));
#else
  #define ESP_MUTEX_INIT(mUtEx) vPortCPUInitializeMutex(&(mUtEx));
#endif
#endif


#define HOSTED_CALLOC(struct_name, buff, nbytes, gotosym) do {    \
    buff = (struct_name *)g_h.funcs->_h_calloc(1, nbytes);        \
    if (!buff) {                                                  \
        printf("%s, Failed to allocate memory \n", __func__);     \
        goto gotosym;                                             \
    }                                                             \
} while(0);

#define HOSTED_FREE(buff) if (buff) { g_h.funcs->_h_free(buff); buff = NULL; }

/* -------- Create handle ------- */
#define HOSTED_CREATE_HANDLE(tYPE, hANDLE) {                                   \
	hANDLE = (tYPE *)g_h.funcs->_h_malloc(sizeof(tYPE));                       \
	if (!hANDLE) {                                                             \
		printf("%s:%u Mem alloc fail while create handle\n", __func__,__LINE__); \
		return NULL;                                                           \
	}                                                                          \
}


#define ESP_NW_PKT_HEADROOM H_ESP_PAYLOAD_HEADER_OFFSET

/* -------- Free handle ------- */

/* Driver Handle */
struct serial_drv_handle_t;

/* Timer handle */
struct timer_handle_t;

uint32_t esp_log_timestamp(void);
uint32_t esp_get_free_heap_size( void );
uint64_t esp_timer_get_time(void);

#endif /*__OS_WRAPPER_H*/
