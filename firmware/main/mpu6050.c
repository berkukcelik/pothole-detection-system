#include "mpu6050.h"
#include "esp_log.h"

#define MPU6050_ADDR      0x68
#define REG_WHO_AM_I      0x75
#define REG_PWR_MGMT_1    0x6B
#define REG_ACCEL_XOUT_H  0x3B
#define ACCEL_SCALE       16384.0f
#define I2C_TIMEOUT_MS    100

static const char *TAG = "mpu6050";
static i2c_master_dev_handle_t mpu;

esp_err_t mpu6050_init(i2c_master_bus_handle_t bus)
{
    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = MPU6050_ADDR,
        .scl_speed_hz = 100000,
    };
    esp_err_t err = i2c_master_bus_add_device(bus, &dev_config, &mpu);
    if (err != ESP_OK) {
        return err;
    }

    uint8_t reg = REG_WHO_AM_I;
    uint8_t who_am_i = 0;
    err = i2c_master_transmit_receive(mpu, &reg, 1, &who_am_i, 1, I2C_TIMEOUT_MS);
    if (err != ESP_OK) {
        return err;
    }

    // 0x68 is the original chip, 0x72 is the clone on my module
    if (who_am_i != 0x68 && who_am_i != 0x72) {
        ESP_LOGE(TAG, "Unexpected WHO_AM_I: 0x%02X", who_am_i);
        return ESP_ERR_NOT_FOUND;
    }
    ESP_LOGI(TAG, "Found sensor, WHO_AM_I = 0x%02X", who_am_i);

    // Wake up the sensor, the original chip starts in sleep mode
    uint8_t wake_cmd[2] = {REG_PWR_MGMT_1, 0x00};
    return i2c_master_transmit(mpu, wake_cmd, 2, I2C_TIMEOUT_MS);
}

esp_err_t mpu6050_read_accel(mpu6050_accel_t *accel)
{
    uint8_t reg = REG_ACCEL_XOUT_H;
    uint8_t data[6];

    esp_err_t err = i2c_master_transmit_receive(mpu, &reg, 1, data, 6, I2C_TIMEOUT_MS);
    if (err != ESP_OK) {
        return err;
    }

    int16_t raw_x = (int16_t)((data[0] << 8) | data[1]);
    int16_t raw_y = (int16_t)((data[2] << 8) | data[3]);
    int16_t raw_z = (int16_t)((data[4] << 8) | data[5]);

    accel->x = raw_x / ACCEL_SCALE;
    accel->y = raw_y / ACCEL_SCALE;
    accel->z = raw_z / ACCEL_SCALE;

    return ESP_OK;
}