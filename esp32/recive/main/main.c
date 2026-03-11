#include <stdint.h>
#include <stdio.h>
#include "esp_now.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "freertos/task.h"
#include "esp_mac.h"
#include <driver/gpio.h>
#include <driver/uart.h>

#define TXD_PIN (GPIO_NUM_17)
#define RXD_PIN (GPIO_NUM_16)
#define UART_PORT UART_NUM_2

void init_uart(){
  uart_config_t uart_config = {.baud_rate = 115200,
                               .data_bits = UART_DATA_8_BITS,
                               .parity = UART_PARITY_DISABLE,
                               .stop_bits = UART_STOP_BITS_1,
                               .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};

  // 2. Install driver and set pins
  uart_driver_install(UART_PORT, 1024 * 2, 0, 0, NULL, 0);
  uart_param_config(UART_PORT, &uart_config);
  uart_set_pin(UART_PORT, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE,
               UART_PIN_NO_CHANGE);
}

uint8_t data_send[2] = {0,0};
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

    uint8_t packet = 1;
    if (gx < 0) {
      gx = -gx;
    }
    if (gx > 200) {
      packet = packet | 1 << 1;
    }
    if (gy < 0) {
      gy = -gy;
    }
    if (gy > 200) {
      packet = packet | 1 << 2;
    }
    if (gz < 0) {
      gz = -gz;
    }
    if (gz > 200) {
      packet = packet | 1 << 3;
    }
    packet = packet | (data[14] & 1) << 4;
    if (data[15]){
      data_send[0] = packet;
    } else {
      data_send[1] = packet;
    }
    // printf("%f %f %f\n", gx, gy, gz);

    if ((data_send[0] & 1) && (data_send[1] & 1)){
        if (data_send[0] >> 1 || data_send[1] >> 1){
          uart_write_bytes(UART_PORT, data_send, 2);

          printf(">Received from MAC %02X:%02X:%02X:%02X:%02X:%02X ", mac[0],
                 mac[1], mac[2], mac[3], mac[4], mac[5]);
          for (int8_t i = 8 - 1; i >= 0; i--) {
            printf("%d", (data_send[0] >> i) & 1);
          }
          printf(" ");
          for (int8_t i = 8 - 1; i >= 0; i--) {
            printf("%d", (data_send[1] >> i) & 1);
          }
          printf("\n");
        }
      data_send[0] = 0;
      data_send[1] = 0;
    }

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
    // gpio_reset_pin(LED_GPIO);                 // Reset to default state
    // gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);  // Set as output
    wifi_init();
    init_uart();

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