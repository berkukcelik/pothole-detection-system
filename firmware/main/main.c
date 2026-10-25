#include "driver/i2c_master.h" 
#include "esp_log.h"

#define I2C_SDA_PIN 21 
#define I2C_SCL_PIN 22


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
    ESP_LOGI(TAG,"Scanning I2C bus"); 
    
    for (int addr = 1; addr < 127; addr++) {
        if (i2c_master_probe(bus, addr, 50) == ESP_OK) {
            ESP_LOGI(TAG, "Found device at 0x%02X", addr);
        }
    }

    ESP_LOGI(TAG, "Scan done");

}