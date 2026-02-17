/*
 Copyright (C) AC SOFTWARE SP. Z O.O.

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.
 */

#ifndef SP111_H_
#define SP111_H_

#define ESP8266_SUPLA_PROTO_VERSION 16

#define AP_SSID "SP111"

#define B_RELAY1_PORT 12
#define B_CFG_PORT 0
#define B_HLW_CF_PORT 5
#define B_HLW_CF1_PORT 14
#define B_HLW_SEL_PORT 13
#define LED_RED_PORT 2

#define ELECTRICITY_METER_COUNT 1
#define ELECTRICITY_METER_CHANNEL_OFFSET 1

#define BOARD_CFG_HTML_TEMPLATE
#define BOARD_INTR_HANDLER \
  if (supla_esp_board_intr_handler(gpio_status)) return
#define BOARD_ESP_STARTING supla_esp_board_starting();

char *ICACHE_FLASH_ATTR supla_esp_board_cfg_html_template(
    char dev_name[25], const char mac[6], const char data_saved);

void ICACHE_FLASH_ATTR supla_esp_board_starting(void);
uint8 ICACHE_FLASH_ATTR supla_esp_board_get_measurements(
    unsigned char channel_number, TElectricityMeter_ExtendedValue_V2 *ev);
uint8 supla_esp_board_intr_handler(uint32 gpio_status);

void supla_esp_board_send_channel_values_with_delay(void *srpc);

#endif
