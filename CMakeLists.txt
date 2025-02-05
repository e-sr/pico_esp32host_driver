
project(pico_esp32host_driver)
# Pull in PICO SDK (must be before project)

set(PICO_ESP32HOST_DRIVER_PATH /home/enzo/Documents/embedded/Raspberry/esp32_host/submodules/esp-hosted/esp-hosted-stm32-spi-prerelease)
set(PICO_ESP32HOST_DRIVER_HOST_PATH ${PICO_ESP32HOST_DRIVER_PATH}/host)
set(PICO_ESP32HOST_DRIVER_HOST_EXTRA_PATH ${PICO_ESP32HOST_DRIVER_PATH}/host/extra_components)

# base driver without spi,or... bus
pico_add_library(pico_esp32host_driver_base NOFLAG)

target_sources(pico_esp32host_driver_base
    INTERFACE
    "${PICO_ESP32HOST_DRIVER_HOST_PATH}/esp_hosted_host_init.c"
    "${PICO_ESP32HOST_DRIVER_HOST_PATH}/api/src/esp_wifi_weak.c"
    "${PICO_ESP32HOST_DRIVER_HOST_PATH}/api/src/esp_hosted_api.c"
    "${PICO_ESP32HOST_DRIVER_HOST_PATH}/utils/common.c"
    "${PICO_ESP32HOST_DRIVER_HOST_PATH}/utils/util.c"
    "${PICO_ESP32HOST_DRIVER_HOST_PATH}/utils/stats.c"
    #rpc files
    "${PICO_ESP32HOST_DRIVER_HOST_PATH}/drivers/rpc/core/rpc_core.c"
    "${PICO_ESP32HOST_DRIVER_HOST_PATH}/drivers/rpc/core/rpc_req.c"
    "${PICO_ESP32HOST_DRIVER_HOST_PATH}/drivers/rpc/core/rpc_rsp.c"
    "${PICO_ESP32HOST_DRIVER_HOST_PATH}/drivers/rpc/core/rpc_evt.c"
    "${PICO_ESP32HOST_DRIVER_HOST_PATH}/drivers/rpc/slaveif/rpc_slave_if.c"
    "${PICO_ESP32HOST_DRIVER_HOST_PATH}/drivers/rpc/wrap/rpc_wrap.c"
    #transport files
    "${PICO_ESP32HOST_DRIVER_HOST_PATH}/drivers/transport/spi/spi_drv.c"
    "${PICO_ESP32HOST_DRIVER_HOST_PATH}/drivers/transport/transport_drv.c"
    "${PICO_ESP32HOST_DRIVER_HOST_PATH}/drivers/bt/hci_stub_drv.c"
    # "${PICO_ESP32HOST_DRIVER_PATH}/drivers/serial/serial_ll_if.c"
    # "${PICO_ESP32HOST_DRIVER_PATH}/drivers/serial/serial_drv.c"
    #slave and host common files
    "${PICO_ESP32HOST_DRIVER_PATH}/common/protobuf-c/protobuf-c.c"
    "${PICO_ESP32HOST_DRIVER_PATH}/common/proto/esp_hosted_rpc.pb-c.c"
    # extra components
    "${PICO_ESP32HOST_DRIVER_HOST_EXTRA_PATH}/mempool/mempool.c"
)

target_include_directories(pico_esp32host_driver_base_headers SYSTEM INTERFACE
    "${PICO_ESP32HOST_DRIVER_HOST_PATH}"
    "${PICO_ESP32HOST_DRIVER_HOST_PATH}/api"
    "${PICO_ESP32HOST_DRIVER_HOST_PATH}/api/include"
    "${PICO_ESP32HOST_DRIVER_HOST_PATH}/utils"
    #rpc
    "${PICO_ESP32HOST_DRIVER_HOST_PATH}/drivers/rpc/core"
    "${PICO_ESP32HOST_DRIVER_HOST_PATH}/drivers/rpc/slaveif"
    "${PICO_ESP32HOST_DRIVER_HOST_PATH}/drivers/rpc/wrap"
    # transport
    "${PICO_ESP32HOST_DRIVER_HOST_PATH}/drivers/transport"
    "${PICO_ESP32HOST_DRIVER_HOST_PATH}/drivers/transport/spi"
    "${PICO_ESP32HOST_DRIVER_HOST_PATH}/drivers/bt/"
    #"${PICO_ESP32HOST_DRIVER_PATH}/drivers/transport/sdio"
    #"${PICO_ESP32HOST_DRIVER_PATH}/drivers/serial"
    #slave and host common files
    "${PICO_ESP32HOST_DRIVER_PATH}/common/include"
    "${PICO_ESP32HOST_DRIVER_PATH}/common"
    "${PICO_ESP32HOST_DRIVER_PATH}/common/proto"
    # extra components
    "${PICO_ESP32HOST_DRIVER_HOST_EXTRA_PATH}/mempool"
    "${PICO_ESP32HOST_DRIVER_HOST_EXTRA_PATH}/esp_system" #for esp_task.h
    "${PICO_ESP32HOST_DRIVER_HOST_EXTRA_PATH}/utils"
    "${PICO_ESP32HOST_DRIVER_HOST_EXTRA_PATH}/esp_event/include"
    "${PICO_ESP32HOST_DRIVER_HOST_EXTRA_PATH}/esp_wifi/include"
    "${PICO_ESP32HOST_DRIVER_HOST_EXTRA_PATH}/esp_wifi_remote/include"
    "${PICO_ESP32HOST_DRIVER_HOST_EXTRA_PATH}/esp_netif/include"
    "${PICO_ESP32HOST_DRIVER_HOST_EXTRA_PATH}/log"

)


pico_add_library(pico_esp32host_freertos_driver NOFLAG)
target_sources(pico_esp32host_freertos_driver
    INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/freertos/os_wrapper.c"
    "${CMAKE_CURRENT_LIST_DIR}/freertos/esp_hosted_ota.c"
    "${CMAKE_CURRENT_LIST_DIR}/freertos/esp_hosted_config.c"
)

#add_subdirectory(${PICO_ESP32HOST_DRIVER_PATH} esp-hosted)

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

