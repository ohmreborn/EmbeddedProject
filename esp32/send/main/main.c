// ESP NOW TX - send message for ESP IDF version 5.0 and higher
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include <driver/i2c.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/lock.h>
#include <sys/param.h>
#include <unistd.h>

#include <string.h>
#include "esp_now.h"
#include "esp_mac.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "hal/gpio_types.h"
#include "nvs_flash.h"
#include "freertos/task.h"
#include "soc/gpio_num.h"

#define I2C_NUM I2C_NUM_0
#define I2C_SCL GPIO_NUM_22
#define I2C_SDA GPIO_NUM_21
#define MPU_ADDR 0x68

#define BUTTON_PIN GPIO_NUM_19 // scroll/back

// Receiver MAC Address
// uint8_t mac_destination[6] = {0xe8, 0x68, 0xe7, 0x23, 0x82, 0x1c};
uint8_t mac_destination[6] = {0x30, 0xae, 0xa4, 0x1a, 0x28, 0x20};

static esp_err_t i2c_master_init(void) {
  int i2c_master_port = I2C_NUM;

  i2c_config_t conf = {
      .mode = I2C_MODE_MASTER,
      .sda_io_num = I2C_SDA,
      .scl_io_num = I2C_SCL,
      .sda_pullup_en = GPIO_PULLUP_ENABLE,
      .scl_pullup_en = GPIO_PULLUP_ENABLE,
      .master.clk_speed = 400000,
  };
  i2c_param_config(i2c_master_port, &conf);
  return i2c_driver_install(i2c_master_port, conf.mode, 0, 0, 0);
}

esp_err_t mpuReadfromReg(uint8_t Reg, uint8_t *ReadBuffer, size_t len) {
  return (i2c_master_write_read_device(I2C_NUM, MPU_ADDR, &Reg, 1, ReadBuffer,
                                       len, 2000));
}

esp_err_t mpuWriteReg(uint8_t Reg, uint8_t data) {
  uint8_t writeBuf[2]; // writeBuf[len+1];
  writeBuf[0] = Reg;
  writeBuf[1] = data;
  return (i2c_master_write_to_device(I2C_NUM, MPU_ADDR, writeBuf, 2, 1000));
}

void mpu_6050_init(){
  uint8_t data[10];
  ESP_ERROR_CHECK(i2c_master_init());
  ESP_LOGI("start", "INIT success full");
  mpuReadfromReg(0x75, data, 1);
  ESP_LOGI("start", "%x", data[0]);
  mpuWriteReg(0x6B, 0);
  mpuWriteReg(0x19, 7); // sample rate 1KHz
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
    esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE); // WiFi on channel 1
}


void gpio_init(gpio_num_t pin)
{
  gpio_config_t io_conf = {0};
  io_conf.intr_type = GPIO_INTR_DISABLE;
  io_conf.mode = GPIO_MODE_INPUT;
  io_conf.pin_bit_mask = (1ULL << pin); 
  io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
  io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
  gpio_config(&io_conf);
}


void app_main(void)
{
  gpio_init(BUTTON_PIN);
  gpio_init(GPIO_NUM_10);
  mpu_6050_init();
  wifi_init();

  // Find local mac address
  uint8_t my_esp_mac[6] = {0};
  esp_read_mac(my_esp_mac, ESP_MAC_WIFI_STA);
  printf("Local MAC address: %02X:%02X:%02X:%02X:%02X:%02X\n", my_esp_mac[0],
         my_esp_mac[1], my_esp_mac[2], my_esp_mac[3], my_esp_mac[4],
         my_esp_mac[5]);

  // ESP-NOW initiation and register a callback function that will be called
  esp_now_init();
  // esp_now_register_send_cb(on_data_sent);

  // Add a peer device with which ESP32 can communicate
  esp_now_peer_info_t peer = {0};
  memcpy(peer.peer_addr, mac_destination, 6);
  peer.channel = 1;
  peer.encrypt = false;
  esp_now_add_peer(&peer);

  uint8_t curr_bttn = 0;
  uint8_t prev_bttn = 0;
  while (1) {
    uint8_t data[16];
    mpuReadfromReg(0x3B, data, 14);
    curr_bttn = gpio_get_level(BUTTON_PIN);
    data[14] = curr_bttn && !prev_bttn;
    prev_bttn = curr_bttn;
    data[15] = gpio_get_level(GPIO_NUM_10);
    esp_now_send(mac_destination, (uint8_t *)data, 16);
    // printf("%d\n", data[15]);
    vTaskDelay(pdMS_TO_TICKS(50));
    }
}