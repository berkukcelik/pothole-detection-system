#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c_master.h"
#include "esp_log.h"
#include "mpu6050.h"

#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22

static const char *TAG = "main";

void app_main(void)
{
    i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = I2C_SDA_PIN,
        .scl_io_num = I2C_SCL_PIN,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };
    i2c_master_bus_handle_t bus;
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &bus));

    ESP_ERROR_CHECK(mpu6050_init(bus));

    mpu6050_accel_t accel;
    while (1) {
        if (mpu6050_read_accel(&accel) == ESP_OK) {
            ESP_LOGI(TAG, "ax=%5.2f g  ay=%5.2f g  az=%5.2f g", accel.x, accel.y, accel.z);
        } else {
            ESP_LOGW(TAG, "Failed to read acceleration");
        }
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}