
project(pico_esp32host_driver)
# Pull in PICO SDK (must be before project)

set(ESP_HOSTED_MCU_PATH esp_submodules/esp-hosted-mcu)
set(ESP_HOSTED_MCU_HOST_PATH ${ESP_HOSTED_MCU_PATH}/host)
set(ESP_HOSTED_MCU_EXTRA_PATH esp_submodules/extra_components)

# base driver without spi,or... bus
pico_add_library(pico_esp32host_driver_base NOFLAG)

target_sources(pico_esp32host_driver_base
    INTERFACE
    "${ESP_HOSTED_MCU_HOST_PATH}/esp_hosted_host_init.c"
    "${ESP_HOSTED_MCU_HOST_PATH}/api/src/esp_wifi_weak.c"
    "${ESP_HOSTED_MCU_HOST_PATH}/api/src/esp_hosted_api.c"
    "${ESP_HOSTED_MCU_HOST_PATH}/utils/common.c"
    "${ESP_HOSTED_MCU_HOST_PATH}/utils/util.c"
    "${ESP_HOSTED_MCU_HOST_PATH}/utils/stats.c"
    #rpc files
    "${ESP_HOSTED_MCU_HOST_PATH}/drivers/rpc/core/rpc_core.c"
    "${ESP_HOSTED_MCU_HOST_PATH}/drivers/rpc/core/rpc_req.c"
    "${ESP_HOSTED_MCU_HOST_PATH}/drivers/rpc/core/rpc_rsp.c"
    "${ESP_HOSTED_MCU_HOST_PATH}/drivers/rpc/core/rpc_evt.c"
    "${ESP_HOSTED_MCU_HOST_PATH}/drivers/rpc/slaveif/rpc_slave_if.c"
    "${ESP_HOSTED_MCU_HOST_PATH}/drivers/rpc/wrap/rpc_wrap.c"
    #transport files
    "${ESP_HOSTED_MCU_HOST_PATH}/drivers/transport/spi/spi_drv.c"
    "${ESP_HOSTED_MCU_HOST_PATH}/drivers/transport/transport_drv.c"
    "${ESP_HOSTED_MCU_HOST_PATH}/drivers/bt/hci_stub_drv.c"
    # "${ESP_HOSTED_MCU_PATH}/drivers/serial/serial_ll_if.c"
    # "${ESP_HOSTED_MCU_PATH}/drivers/serial/serial_drv.c"
    #slave and host common files
    "${ESP_HOSTED_MCU_PATH}/common/protobuf-c/protobuf-c.c"
    "${ESP_HOSTED_MCU_PATH}/common/proto/esp_hosted_rpc.pb-c.c"
    # extra components
    "${ESP_HOSTED_MCU_EXTRA_PATH}/mempool/mempool.c"
)

target_include_directories(pico_esp32host_driver_base_headers SYSTEM INTERFACE
    "${ESP_HOSTED_MCU_HOST_PATH}"
    "${ESP_HOSTED_MCU_HOST_PATH}/api"
    "${ESP_HOSTED_MCU_HOST_PATH}/api/include"
    "${ESP_HOSTED_MCU_HOST_PATH}/utils"
    #rpc
    "${ESP_HOSTED_MCU_HOST_PATH}/drivers/rpc/core"
    "${ESP_HOSTED_MCU_HOST_PATH}/drivers/rpc/slaveif"
    "${ESP_HOSTED_MCU_HOST_PATH}/drivers/rpc/wrap"
    # transport
    "${ESP_HOSTED_MCU_HOST_PATH}/drivers/transport"
    "${ESP_HOSTED_MCU_HOST_PATH}/drivers/transport/spi"
    "${ESP_HOSTED_MCU_HOST_PATH}/drivers/bt/"
    #"${ESP_HOSTED_MCU_PATH}/drivers/transport/sdio"
    #"${ESP_HOSTED_MCU_PATH}/drivers/serial"
    #slave and host common files
    "${ESP_HOSTED_MCU_PATH}/common/include"
    "${ESP_HOSTED_MCU_PATH}/common"
    "${ESP_HOSTED_MCU_PATH}/common/proto"
    # extra components
    "${ESP_HOSTED_MCU_EXTRA_PATH}/mempool"
    "${ESP_HOSTED_MCU_EXTRA_PATH}/esp_system" #for esp_task.h
    "${ESP_HOSTED_MCU_EXTRA_PATH}/utils"
    "${ESP_HOSTED_MCU_EXTRA_PATH}/esp_event/include"
    "${ESP_HOSTED_MCU_EXTRA_PATH}/esp_wifi/include"
    "${ESP_HOSTED_MCU_EXTRA_PATH}/esp_wifi_remote/include"
    "${ESP_HOSTED_MCU_EXTRA_PATH}/esp_netif/include"
    "${ESP_HOSTED_MCU_EXTRA_PATH}/log"
    #slave sdkconfig
    "${ESP_HOSTED_MCU_PATH}/slave/build/config/sdkconfig.h"
)


pico_add_library(pico_esp32host_freertos_driver NOFLAG)
target_sources(pico_esp32host_freertos_driver
    INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/freertos/os_wrapper.c"
    "${CMAKE_CURRENT_LIST_DIR}/freertos/esp_hosted_ota.c"
    "${CMAKE_CURRENT_LIST_DIR}/freertos/esp_hosted_config.c"
)

#add_subdirectory(${ESP_HOSTED_MCU_PATH} esp-hosted)

target_include_directories(pico_esp32host_freertos_driver SYSTEM INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/freertos/platform_include
    ${CMAKE_CURRENT_LIST_DIR}/freertos
    ${CMAKE_CURRENT_LIST_DIR}/.

)
pico_mirrored_target_link_libraries(pico_esp32host_freertos_driver INTERFACE
    pico_esp32host_driver_base
    pico_esp32host_driver
)

# # this adds async_context integration to cyw43_driver
# pico_add_library(pico_cyw43_driver NOFLAG)
# target_sources(pico_cyw43_driver INTERFACE
#     cyw43_driver.c)
# target_include_directories(pico_cyw43_driver_headers SYSTEM INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
# pico_mirrored_target_link_libraries(pico_cyw43_driver INTERFACE cyw43_driver)

# pico_esp32host_driver_spi is pico_esp32host_driver plus spi specific bus implementation
pico_add_library(pico_esp32host_freertos_driver_spi NOFLAG)
target_sources(pico_esp32host_freertos_driver_spi INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/freertos/spi_wrapper.c
)
pico_mirrored_target_link_libraries(pico_esp32host_freertos_driver_spi INTERFACE
    esp32host_core
    pico_esp32host_freertos_driver
    hardware_spi
    hardware_dma
    hardware_exception
)


pico_promote_common_scope_vars()

