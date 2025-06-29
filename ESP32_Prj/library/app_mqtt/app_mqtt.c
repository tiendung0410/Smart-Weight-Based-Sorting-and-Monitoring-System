#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "lwip/err.h"
#include "lwip/sys.h"


#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "cJSON.h"

static const char *TAG = "MQTT_APP";
ESP_EVENT_DEFINE_BASE(APP_MQTT_EVENT);
static esp_mqtt_client_handle_t client;


/* buffer gửi dữ liệu lên server */
char payload[512];

static void (*mqtt_data_handler_cb)(char *) = NULL;

// nhận json từ server -> parse -> lưu vào biến
void mqtt_data_handler(char *data)
{
    if (mqtt_data_handler_cb != NULL)
    {
        mqtt_data_handler_cb(data);
    }
    else
    {
        printf("mqtt_data_handler_cb is NULL!\n");
    }
}

//test in ra mh
// void test_print(void)
// {
//     printf(
//         "mode: %d\n"
//         "box1_red_preset:%d\nbox1_blue_preset:%d\nbox1_yellow_preset:%d\n"
//         "box1_red:%d\nbox1_blue:%d\nbox1_yellow:%d\nbox1_status:%s\n"
//         "box2_red_preset:%d\nbox2_blue_preset:%d\nbox2_yellow_preset:%d\n"
//         "box2_red:%d\nbox2_blue:%d\nbox2_yellow:%d\nbox2_status:%s\n"
//         "box3_red_preset:%d\nbox3_blue_preset:%d\nbox3_yellow_preset:%d\n"
//         "box3_red:%d\nbox3_blue:%d\nbox3_yellow:%d\nbox3_status:%s\n",
//         system_mode,
//         box1_red_preset, box1_blue_preset, box1_yellow_preset, box1_red, box1_blue, box1_yellow, box1_status,
//         box2_red_preset, box2_blue_preset, box2_yellow_preset, box2_red, box2_blue, box2_yellow, box2_status,
//         box3_red_preset, box3_blue_preset, box3_yellow_preset, box3_red, box3_blue, box3_yellow, box3_status
//     );
// }

static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event)
{
    int msg_id;
    // your_context_t *context = event->context;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            msg_id = esp_mqtt_client_subscribe(client, "v1/devices/me/telemetry", 0);
            // msg_id = esp_mqtt_client_subscribe(client, "v1/devices/me/rpc/request/+", 0);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            mqtt_data_handler(event->data);
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
    return ESP_OK;
}

void mqtt_app_start(void)
{
    const esp_mqtt_client_config_t mqtt_cfg = {
        //địa chỉ của broker mqtt
        .uri = "mqtt://thingsboard.cloud",
        .username = "bGwkwGkqGbPAfi08Ux2D",
        //hàm xử lí skien
        .event_handle = mqtt_event_handler,
    };

    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_start(client);
}


void mqtt_app_publish(char *topic, char *data, size_t len)
{
    esp_mqtt_client_publish(client,topic,data,len,1,0);
}

void mqtt_app_subscribe(char *topic)
{
    esp_mqtt_client_subscribe(client,topic,1);
}

//gửi data len cloud -> chia làm 2 mode
void send_data_to_server(void)
{
    memset(payload, 0, sizeof(payload));

    // if(system_mode == MODE1)
    // {
    //     snprintf(payload, sizeof(payload) - 1,
    //         "{\"mode\":\"%s\","
    //         "\"mode1_box1\":%d,"
    //         "\"mode1_box2\":%d,"
    //         "\"mode1_box3\":%d}",
    //         "false", mode1_box1, mode1_box2, mode1_box3
    //     );
    // }
    // else if(system_mode == MODE2)
    // {
    //     snprintf(payload, sizeof(payload) - 1,
    //         "{\"mode\":\"%s\","
    //         "\"box1_red_preset\":%d,\"box1_blue_preset\":%d,\"box1_yellow_preset\":%d,"
    //         "\"box1_red\":%d,\"box1_blue\":%d,\"box1_yellow\":%d,\"box1_status\":\"%s\","
    //         "\"box2_red_preset\":%d,\"box2_blue_preset\":%d,\"box2_yellow_preset\":%d,"
    //         "\"box2_red\":%d,\"box2_blue\":%d,\"box2_yellow\":%d,\"box2_status\":\"%s\","
    //         "\"box3_red_preset\":%d,\"box3_blue_preset\":%d,\"box3_yellow_preset\":%d,"
    //         "\"box3_red\":%d,\"box3_blue\":%d,\"box3_yellow\":%d,\"box3_status\":\"%s\"}",
    //         "true",
    //         box1_red_preset, box1_blue_preset, box1_yellow_preset, box1_red, box1_blue, box1_yellow, box1_status,
    //         box2_red_preset, box2_blue_preset, box2_yellow_preset, box2_red, box2_blue, box2_yellow, box2_status,
    //         box3_red_preset, box3_blue_preset, box3_yellow_preset, box3_red, box3_blue, box3_yellow, box3_status
    //     );
    // }

    
    mqtt_app_publish("v1/devices/me/attributes", payload, strlen(payload));
}

void mqtt_app_set_data_handler_cb(void (*cb)(char *))
{
    mqtt_data_handler_cb = cb;
}

