add_library(esp_extra_components INTERFACE)
add_subdirectory(esp_event)
add_subdirectory(esp_netif)
add_subdirectory(esp_system)
add_subdirectory(esp_wifi)
add_subdirectory(esp_wifi_remote)
add_subdirectory(utils)
add_subdirectory(mempool)
add_subdirectory(log)

target_link_libraries(esp_extra_components INTERFACE
    esp_event
    esp_netif
    esp_system
    esp_wifi
    esp_wifi_remote
    esp_timer
    utils
)

# soc esp_event esp_netif esp_timer driver esp_wifi bt esp_http_client
