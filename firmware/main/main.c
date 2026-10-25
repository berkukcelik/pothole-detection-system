#include "driver/i2c_master.h" 
#include "esp_log.h"

#define I2C_SDA_PIN 21 
#define I2C_SCL_PIN 22
#define MPU6050_ADDR 0x68 
#define REG_WHO_AM_I  0x75
#define REG_PWR_MGMT_1 0x6B
static const char* TAG = "main"; 

void app_main(void){
    i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = I2C_SDA_PIN,
        .scl_io_num = I2C_SCL_PIN, 
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    }; 
    i2c_master_bus_handle_t bus; 
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config,&bus));

    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7, 
        .device_address = MPU6050_ADDR,
        .scl_speed_hz = 100000,
    }; 

    i2c_master_dev_handle_t mpu; 
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus,&dev_config,&mpu));

    uint8_t reg = REG_WHO_AM_I; 
    uint8_t who_am_i = 0; 
    ESP_ERROR_CHECK(i2c_master_transmit_receive(mpu, &reg, 1, &who_am_i, 1, 100));

    ESP_LOGI(TAG, "WHO_AM_I = 0x%02X", who_am_i);


    // --- Wake up the sensor ---
    uint8_t power = 0;

    reg = REG_PWR_MGMT_1;

    ESP_ERROR_CHECK(i2c_master_transmit_receive(mpu, &reg, 1, &power, 1, 100));
    ESP_LOGI(TAG, "PWR_MGMT_1 before = 0x%02X", power);

    uint8_t wake_cmd[2] = {REG_PWR_MGMT_1, 0x00};
    ESP_ERROR_CHECK(i2c_master_transmit(mpu, wake_cmd, 2, 100));

    ESP_ERROR_CHECK(i2c_master_transmit_receive(mpu, &reg, 1, &power, 1, 100));
    ESP_LOGI(TAG, "PWR_MGMT_1 after = 0x%02X", power);

}