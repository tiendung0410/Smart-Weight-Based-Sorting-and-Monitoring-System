/* i2c - Example

   For other examples please check:
   https://github.com/espressif/esp-idf/tree/master/examples

   See README.md file to get detailed usage of this example.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "sdkconfig.h"
#include "string.h"
#include "app_mqtt.h"
#include "wifi_station.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_log.h"

static const char *TAG = "i2c-example";

#define _I2C_NUMBER(num) I2C_NUM_##num
#define I2C_NUMBER(num) _I2C_NUMBER(num)

#define DATA_LENGTH 512                  /*!< Data buffer length of test buffer */
#define RW_TEST_LENGTH 128               /*!< Data length for r/w test, [0,DATA_LENGTH] */
#define DELAY_TIME_BETWEEN_ITEMS_MS 1000 /*!< delay time between different test items */

#define I2C_SLAVE_SCL_IO 22               /*!< gpio number for i2c slave clock */
#define I2C_SLAVE_SDA_IO 21               /*!< gpio number for i2c slave data */
#define I2C_SLAVE_NUM I2C_NUMBER(0) /*!< I2C port number for slave dev */
#define I2C_SLAVE_TX_BUF_LEN (2 * DATA_LENGTH)              /*!< I2C slave tx buffer size */
#define I2C_SLAVE_RX_BUF_LEN (2 * DATA_LENGTH)              /*!< I2C slave rx buffer size */

#define I2C_MASTER_SCL_IO CONFIG_I2C_MASTER_SCL               /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO CONFIG_I2C_MASTER_SDA               /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUMBER(CONFIG_I2C_MASTER_PORT_NUM) /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ CONFIG_I2C_MASTER_FREQUENCY        /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE 0                           /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0                           /*!< I2C master doesn't need buffer */

#define BH1750_SENSOR_ADDR CONFIG_BH1750_ADDR   /*!< slave address for BH1750 sensor */
#define BH1750_CMD_START CONFIG_BH1750_OPMODE   /*!< Operation mode */
#define ESP_SLAVE_ADDR 0x28 /*!< ESP32 slave address, you can set any 7bit value */
#define WRITE_BIT I2C_MASTER_WRITE              /*!< I2C master write */
#define READ_BIT I2C_MASTER_READ                /*!< I2C master read */
#define ACK_CHECK_EN 0x1                        /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0                       /*!< I2C master will not check ack from slave */
#define ACK_VAL 0x0                             /*!< I2C ack value */
#define NACK_VAL 0x1                            /*!< I2C nack value */

#define WIFI_SSID "DoAnhBatDuocEm" // wifi ssid
#define WIFI_PASSWORD "mot23456"   // wifi password
SemaphoreHandle_t print_mux = NULL;

/**
 * @brief test code to read esp-i2c-slave
 *        We need to fill the buffer of esp slave device, then master can read them out.
 *
 * _______________________________________________________________________________________
 * | start | slave_addr + rd_bit +ack | read n-1 bytes + ack | read 1 byte + nack | stop |
 * --------|--------------------------|----------------------|--------------------|------|
 *
 */
static esp_err_t i2c_master_read_slave(i2c_port_t i2c_num, uint8_t *data_rd, size_t size)
{
    if (size == 0) {
        return ESP_OK;
    }
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (ESP_SLAVE_ADDR << 1) | READ_BIT, ACK_CHECK_EN);
    if (size > 1) {
        i2c_master_read(cmd, data_rd, size - 1, ACK_VAL);
    }
    i2c_master_read_byte(cmd, data_rd + size - 1, NACK_VAL);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

/**
 * @brief Test code to write esp-i2c-slave
 *        Master device write data to slave(both esp32),
 *        the data will be stored in slave buffer.
 *        We can read them out from slave buffer.
 *
 * ___________________________________________________________________
 * | start | slave_addr + wr_bit + ack | write n bytes + ack  | stop |
 * --------|---------------------------|----------------------|------|
 *
 */
static esp_err_t i2c_master_write_slave(i2c_port_t i2c_num, uint8_t *data_wr, size_t size)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (ESP_SLAVE_ADDR << 1) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write(cmd, data_wr, size, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

/**
 * @brief test code to operate on BH1750 sensor
 *
 * 1. set operation mode(e.g One time L-resolution mode)
 * _________________________________________________________________
 * | start | slave_addr + wr_bit + ack | write 1 byte + ack  | stop |
 * --------|---------------------------|---------------------|------|
 * 2. wait more than 24 ms
 * 3. read data
 * ______________________________________________________________________________________
 * | start | slave_addr + rd_bit + ack | read 1 byte + ack  | read 1 byte + nack | stop |
 * --------|---------------------------|--------------------|--------------------|------|
 */
static esp_err_t i2c_master_sensor_test(i2c_port_t i2c_num, uint8_t *data_h, uint8_t *data_l)
{
    int ret;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, BH1750_SENSOR_ADDR << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, BH1750_CMD_START, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret != ESP_OK) {
        return ret;
    }
    vTaskDelay(30 / portTICK_RATE_MS);
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, BH1750_SENSOR_ADDR << 1 | READ_BIT, ACK_CHECK_EN);
    i2c_master_read_byte(cmd, data_h, ACK_VAL);
    i2c_master_read_byte(cmd, data_l, NACK_VAL);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

/**
 * @brief i2c master initialization
 */
static esp_err_t i2c_master_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    i2c_param_config(i2c_master_port, &conf);
    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

/**
 * @brief i2c slave initialization
 */
static esp_err_t i2c_slave_init(void)
{
    int i2c_slave_port = I2C_SLAVE_NUM;
    i2c_config_t conf_slave;
    conf_slave.sda_io_num = I2C_SLAVE_SDA_IO;
    conf_slave.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf_slave.scl_io_num = I2C_SLAVE_SCL_IO;
    conf_slave.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf_slave.mode = I2C_MODE_SLAVE;
    conf_slave.slave.addr_10bit_en = 0;
    conf_slave.slave.slave_addr = ESP_SLAVE_ADDR;
    i2c_param_config(i2c_slave_port, &conf_slave);
    return i2c_driver_install(i2c_slave_port, conf_slave.mode, I2C_SLAVE_RX_BUF_LEN, I2C_SLAVE_TX_BUF_LEN, 0);
}
/* @brief i2c slave task to handle data from master
 *        This task will be created in app_main() function.
 *        It will read data from master and process it.
 */

static uint8_t calculate_checksum(uint8_t *data, int length) {
    uint8_t checksum = 0;
    for (int i = 0; i < length; i++) {
        checksum ^= data[i];
    }
    return checksum;
}

// static void i2c_slave_task(void *arg)
// {
//     uint8_t data[256];
//     // Chờ nhận dữ liệu từ master
//     while (1) {
//         ESP_LOGI(TAG, "Waiting for data from master...");
//         // Đọc dữ liệu từ slave buffer
//         memset(data, 0, sizeof(data));
//         int size = i2c_slave_read_buffer(I2C_SLAVE_NUM, data, sizeof(data), 200 / portTICK_PERIOD_MS);
//         ESP_LOGI(TAG, "Received %d bytes: '%s'", size - 1, (char *)data);
//         if (size > 1) 
//         {
//             uint8_t received_checksum = data[size - 1];  // Lấy checksum từ dữ liệu nhận được
//             uint8_t computed_checksum = calculate_checksum(data, size - 1); // Tính toán checksum từ dữ liệu nhận được
//             if (computed_checksum == received_checksum) 
//             {
//                 data[size - 1] = '\0';  // Xóa checksum, giữ lại chuỗi gốc
//                 ESP_LOGI(TAG, "Received %d bytes: '%s'", size - 1, (char *)data);
//                 // publish data lên MQTT broker
//                 mqtt_app_publish("v1/devices/me/telemetry", (char *)data, size);                
//             } else 
//             {
//                 ESP_LOGE(TAG, "❌ Checksum MISMATCH! Received: %02X, Expected: %02X", received_checksum, computed_checksum);
//             }

//         }
//         vTaskDelay(100 / portTICK_PERIOD_MS);
//     }
// }
static void i2c_slave_task(void *arg)
{
    uint8_t data[256];
    uint8_t assembled_data[512];  // Buffer lớn hơn để chứa dữ liệu ghép lại
    uint16_t assembled_length = 0;
    bool receiving_json = false;
    uint32_t last_receive_time = 0;
    
    while (1) {
        ESP_LOGI(TAG, "Waiting for data...");
        memset(data, 0, sizeof(data));
        
        int size = i2c_slave_read_buffer(I2C_SLAVE_NUM, data, sizeof(data), 200 / portTICK_PERIOD_MS);
        
        if (size > 0) {
            uint32_t current_time = esp_timer_get_time() / 1000; // Thời gian hiện tại tính bằng ms
            
            // Hiển thị dữ liệu nhận được
            ESP_LOGI(TAG, "Received chunk: %d bytes", size);
            
            // Kiểm tra xem đây là đầu của JSON không (bắt đầu bằng '{')
            if (data[0] == '{') {
                // Bắt đầu một gói JSON mới
                memset(assembled_data, 0, sizeof(assembled_data));
                memcpy(assembled_data, data, size);
                assembled_length = size;
                receiving_json = true;
                last_receive_time = current_time;
            } 
            // Nếu đang trong quá trình nhận JSON, ghép thêm dữ liệu mới
            else if (receiving_json && 
                    (current_time - last_receive_time < 500) && // Timeout 500ms
                    (assembled_length + size < sizeof(assembled_data))) {
                
                memcpy(assembled_data + assembled_length, data, size);
                assembled_length += size;
                last_receive_time = current_time;
            }
            
            // Kiểm tra xem JSON đã hoàn thành chưa (kết thúc bằng '}')
            if (receiving_json && 
                assembled_length > 0 &&
                assembled_data[assembled_length - 2] == '}') {
                
                // Dữ liệu JSON hoàn chỉnh, byte cuối cùng là checksum
                uint8_t received_checksum = assembled_data[assembled_length - 1];
                uint8_t computed_checksum = calculate_checksum(assembled_data, assembled_length - 1);
                
                if (computed_checksum == received_checksum) {
                    // Checksum đúng
                    assembled_data[assembled_length - 1] = '\0';  // Xóa checksum
                    ESP_LOGI(TAG, "✅ Complete JSON (%d bytes): '%s'", assembled_length - 1, assembled_data);
                    
                    // Gửi dữ liệu lên MQTT broker
                    mqtt_app_publish("v1/devices/me/telemetry", (char *)assembled_data, assembled_length - 1);
                } else {
                    ESP_LOGE(TAG, "❌ Checksum MISMATCH! Received: %02X, Expected: %02X", 
                             received_checksum, computed_checksum);
                }
                
                // Reset trạng thái để nhận gói tiếp theo
                receiving_json = false;
                assembled_length = 0;
            }
            
            // Kiểm tra timeout để tránh treo khi không nhận đủ dữ liệu
            if (receiving_json && (current_time - last_receive_time > 1000)) {
                ESP_LOGW(TAG, "⚠️ Timeout when receiving JSON data!");
                receiving_json = false;
                assembled_length = 0;
            }
        }
        
        vTaskDelay(1/ portTICK_PERIOD_MS);
    }
}
/**
 * @brief test function to show buffer
 */
static void disp_buf(uint8_t *buf, int len)
{
    int i;
    for (i = 0; i < len; i++) {
        printf("%02x ", buf[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    printf("\n");
}

static void i2c_test_task(void *arg)
{
    int i = 0;
    int ret;
    uint32_t task_idx = (uint32_t)arg;
    uint8_t *data = (uint8_t *)malloc(DATA_LENGTH);
    uint8_t *data_wr = (uint8_t *)malloc(DATA_LENGTH);
    uint8_t *data_rd = (uint8_t *)malloc(DATA_LENGTH);
    uint8_t sensor_data_h, sensor_data_l;
    int cnt = 0;
    while (1) {
        ESP_LOGI(TAG, "TASK[%d] test cnt: %d", task_idx, cnt++);
        ret = i2c_master_sensor_test(I2C_MASTER_NUM, &sensor_data_h, &sensor_data_l);
        xSemaphoreTake(print_mux, portMAX_DELAY);
        if (ret == ESP_ERR_TIMEOUT) {
            ESP_LOGE(TAG, "I2C Timeout");
        } else if (ret == ESP_OK) {
            printf("*******************\n");
            printf("TASK[%d]  MASTER READ SENSOR( BH1750 )\n", task_idx);
            printf("*******************\n");
            printf("data_h: %02x\n", sensor_data_h);
            printf("data_l: %02x\n", sensor_data_l);
            printf("sensor val: %.02f [Lux]\n", (sensor_data_h << 8 | sensor_data_l) / 1.2);
        } else {
            ESP_LOGW(TAG, "%s: No ack, sensor not connected...skip...", esp_err_to_name(ret));
        }
        xSemaphoreGive(print_mux);
        vTaskDelay((DELAY_TIME_BETWEEN_ITEMS_MS * (task_idx + 1)) / portTICK_RATE_MS);
        //---------------------------------------------------
        for (i = 0; i < DATA_LENGTH; i++) {
            data[i] = i;
        }
        xSemaphoreTake(print_mux, portMAX_DELAY);
        size_t d_size = i2c_slave_write_buffer(I2C_SLAVE_NUM, data, RW_TEST_LENGTH, 1000 / portTICK_RATE_MS);
        if (d_size == 0) {
            ESP_LOGW(TAG, "i2c slave tx buffer full");
            ret = i2c_master_read_slave(I2C_MASTER_NUM, data_rd, DATA_LENGTH);
        } else {
            ret = i2c_master_read_slave(I2C_MASTER_NUM, data_rd, RW_TEST_LENGTH);
        }

        if (ret == ESP_ERR_TIMEOUT) {
            ESP_LOGE(TAG, "I2C Timeout");
        } else if (ret == ESP_OK) {
            printf("*******************\n");
            printf("TASK[%d]  MASTER READ FROM SLAVE\n", task_idx);
            printf("*******************\n");
            printf("====TASK[%d] Slave buffer data ====\n", task_idx);
            disp_buf(data, d_size);
            printf("====TASK[%d] Master read ====\n", task_idx);
            disp_buf(data_rd, d_size);
        } else {
            ESP_LOGW(TAG, "TASK[%d] %s: Master read slave error, IO not connected...\n",
                     task_idx, esp_err_to_name(ret));
        }
        xSemaphoreGive(print_mux);
        vTaskDelay((DELAY_TIME_BETWEEN_ITEMS_MS * (task_idx + 1)) / portTICK_RATE_MS);
        //---------------------------------------------------
        int size;
        for (i = 0; i < DATA_LENGTH; i++) {
            data_wr[i] = i + 10;
        }
        xSemaphoreTake(print_mux, portMAX_DELAY);
        //we need to fill the slave buffer so that master can read later
        ret = i2c_master_write_slave(I2C_MASTER_NUM, data_wr, RW_TEST_LENGTH);
        if (ret == ESP_OK) {
            size = i2c_slave_read_buffer(I2C_SLAVE_NUM, data, RW_TEST_LENGTH, 1000 / portTICK_RATE_MS);
        }
        if (ret == ESP_ERR_TIMEOUT) {
            ESP_LOGE(TAG, "I2C Timeout");
        } else if (ret == ESP_OK) {
            printf("*******************\n");
            printf("TASK[%d]  MASTER WRITE TO SLAVE\n", task_idx);
            printf("*******************\n");
            printf("----TASK[%d] Master write ----\n", task_idx);
            disp_buf(data_wr, RW_TEST_LENGTH);
            printf("----TASK[%d] Slave read: [%d] bytes ----\n", task_idx, size);
            disp_buf(data, size);
        } else {
            ESP_LOGW(TAG, "TASK[%d] %s: Master write slave error, IO not connected....\n",
                     task_idx, esp_err_to_name(ret));
        }
        xSemaphoreGive(print_mux);
        vTaskDelay((DELAY_TIME_BETWEEN_ITEMS_MS * (task_idx + 1)) / portTICK_RATE_MS);
    }
    vSemaphoreDelete(print_mux);
    vTaskDelete(NULL);
}

void app_main(void)
{

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    // ESP_ERROR_CHECK(esp_event_handler_register(APP_MQTT_EVENT, ESP_EVENT_ANY_ID, &app_mqtt_event_handler, NULL));

    wifi_setup(WIFI_SSID, WIFI_PASSWORD);
    wifi_init_sta();

    // mqtt_app_set_data_handler_cb(&main_mqtt_data_handler_cb);
    mqtt_app_start();
    // print_mux = xSemaphoreCreateMutex();
    ESP_ERROR_CHECK(i2c_slave_init());
    // ESP_ERROR_CHECK(i2c_master_init());
    // xTaskCreate(i2c_test_task, "i2c_test_task_0", 1024 * 2, (void *)0, 10, NULL);
    // xTaskCreate(i2c_test_task, "i2c_test_task_1", 1024 * 2, (void *)1, 10, NULL);
    xTaskCreate(i2c_slave_task, "i2c_slave_task", 1024 * 8, NULL, 1, NULL);

    
}
