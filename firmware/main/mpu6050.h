#pragma once 

#include "driver/i2c_master.h"
#include "esp_err.h"


typedef struct {
    float x;  
    float y;
    float z;
} mpu6050_accel_t; 

esp_err_t mpu6050_init(i2c_master_bus_handle_t bus);
esp_err_t mpu6050_read_accel(mpu6050_accel_t *accel);
