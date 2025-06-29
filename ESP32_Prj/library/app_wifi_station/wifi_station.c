#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"

#include <esp_system.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include "wifi_station.h"

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1
#define EXAMPLE_ESP_MAXIMUM_RETRY  5

static EventGroupHandle_t s_wifi_event_group;
static int s_retry_num = 0;

static uint8_t ESP_WIFI_SSID[32];
static uint8_t ESP_WIFI_PASS[64];

static const char *TAG = "WIFI_STATION";

//hàm xử lí sự kiện wifi
static void event_handler(void* arg, esp_event_base_t event_base,
    int32_t event_id, void* event_data)
{

    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
    esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
    if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
    esp_wifi_connect();
    s_retry_num++;
    ESP_LOGI(TAG, "retry to connect to the AP");
    } else {
    xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT); //ko connect được wifi thì set bit fail
    }
    ESP_LOGI(TAG,"connect to the AP fail");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
    ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
    ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
    s_retry_num = 0;
    xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT); // connect được wifi thì set bit connected
    }
}

void wifi_init_sta(void)
{
    //tạo event group
    s_wifi_event_group = xEventGroupCreate();

    //init network interface & tạo loop xử lí sự kiện
    ESP_ERROR_CHECK(esp_netif_init());

    // ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    //config và init setting wifi mặc định
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    //tạo handler để register/unregister hàm xử lí sự kiện
    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;

    //đăng ký hàm xử lí sự kiện
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                ESP_EVENT_ANY_ID,
                                &event_handler,
                                NULL,
                                &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                IP_EVENT_STA_GOT_IP,
                                &event_handler,
                                NULL,
                                &instance_got_ip));

    //set wifi mode, set config, start wifi
    wifi_config_t wifi_config = {
        .sta = {
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .capable = true,
                .required = false
            },
        },
    };
    
    // Copy ssid và password
    snprintf((char *)wifi_config.sta.ssid, sizeof(wifi_config.sta.ssid), "%s", ESP_WIFI_SSID);
    snprintf((char *)wifi_config.sta.password, sizeof(wifi_config.sta.password), "%s", ESP_WIFI_PASS);

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start() );

    ESP_LOGI(TAG, "wifi_init_sta finished.");

    //đợi bits từ hàm xử lí sự kiện wifi
    /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
    * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
    WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
    pdFALSE,
    pdFALSE,
    portMAX_DELAY);

    /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
    * happened. */
    if (bits & WIFI_CONNECTED_BIT) {
    ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
    ESP_WIFI_SSID, ESP_WIFI_PASS);
    } else if (bits & WIFI_FAIL_BIT) {
    ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
    ESP_WIFI_SSID, ESP_WIFI_PASS);
    } else {
    ESP_LOGE(TAG, "UNEXPECTED EVENT");
    }

    //hủy đăng kí hàm xử lí sự kiện
    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, instance_got_ip));
    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, instance_any_id));
    vEventGroupDelete(s_wifi_event_group);
}

void wifi_setup(char *ssid, char *password)
{
    snprintf((char *)ESP_WIFI_SSID, sizeof(ESP_WIFI_SSID), "%s", ssid);
    snprintf((char *)ESP_WIFI_PASS, sizeof(ESP_WIFI_PASS), "%s", password);
}

