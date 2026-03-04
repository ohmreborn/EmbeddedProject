#include <stdio.h>
#include "esp_now.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "freertos/task.h"
#include "esp_mac.h"
#include <driver/gpio.h>

#define LED_GPIO 2

static void on_data_recv(const esp_now_recv_info_t *recv_info, const uint8_t *data, int len)
{
    uint8_t *mac = recv_info->src_addr;
    int16_t accelX = (data[0] << 8) | data[1];
    int16_t accelY = (data[2] << 8) | data[3];
    int16_t accelZ = (data[4] << 8) | data[5];

    int16_t gyroX = (data[8] << 8) | data[9];
    int16_t gyroY = (data[10] << 8) | data[11];
    int16_t gyroZ = (data[12] << 8) | data[13];

    float ax = accelX / 16384.0;
    float ay = accelY / 16384.0;
    float az = accelZ / 16384.0;

    float gx = gyroX / 131.0;
    float gy = gyroY / 131.0;
    float gz = gyroZ / 131.0;


    if (gx < 0){
        gx = -gx;
    }
    if (gy < 0){
        gy = -gy;
    }
    if (gz < 0){
        gz = -gz;
    }
    if (gx > 150 || gy > 150 || gz > 150){
        gpio_set_level(LED_GPIO, 1);
    }else{
        gpio_set_level(LED_GPIO, 0);
    }
    // 24:0A:C4:9A:FC:98 left
    printf(">Received from MAC %02X:%02X:%02X:%02X:%02X:%02X AX:%.2f,AY:%.2f,AZ:%.2f,GX:%.2f,GY:%.2f,GZ:%.2f\n",mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], ax, ay,az, gx, gy, gz);
    // printf("Received from MAC %02X:%02X:%02X:%02X:%02X:%02X: %.*s\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], len, (char *)data);
}

void wifi_init()
{
    nvs_flash_init();
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_start();
    esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);
}


void app_main(void)
{
    gpio_reset_pin(LED_GPIO);                 // Reset to default state
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);  // Set as output
    wifi_init();

    esp_now_init();

    // Find local MAC address
    uint8_t mac_l[6] = {0};
    esp_read_mac(mac_l, ESP_MAC_WIFI_STA);
    printf("Local MAC address: \"0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\" \n", mac_l[0], mac_l[1], mac_l[2], mac_l[3], mac_l[4], mac_l[5]);

    // ESP-NOW
    esp_now_register_recv_cb(on_data_recv);
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}