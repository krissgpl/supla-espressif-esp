/*
 Copyright (C) AC SOFTWARE SP. Z O.O.

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef K_RS_MODULE_H_
#define K_RS_MODULE_H_

#define STATE_SECTOR_OFFSET 2		// zmiana sektora zapisu
#define RS_SAVE_STATE_DELAY 500		// zmiana czestotliwosci zapisu

#define ESP8266_SUPLA_PROTO_VERSION 12
#define BOARD_CFG_HTML_TEMPLATE

#define SUPLA_ESP_SOFTVER "2.7.24.0"

#define _ROLLERSHUTTER_SUPPORT

#ifdef __BOARD_k_rs_module
	#define AP_SSID "ROLETY"
#endif

#ifdef __BOARD_k_rs_module_DHT22
	#define DHTSENSOR
   	#define TEMPERATURE_HUMIDITY_CHANNEL 1
	#define AP_SSID "ROLETY_DHT22"
#endif

#ifdef __BOARD_k_rs_module_ds18b20
	#define DS18B20
	#define TEMPERATURE_CHANNEL 1
	#define AP_SSID "ROLETY_DS18B20"
#endif

#define USE_GPIO16_OUTPUT


#define LED_RED_PORT   16
#define WATCHDOG_TIMEOUT_SEC 90

#define BOARD_GPIO_OUTPUT_SET_HI if (supla_last_state == STATE_CONNECTED) {if (port == 20) { \
 	supla_log(LOG_DEBUG, "update, port = %i", port); \
	supla_esp_cfg.FirmwareUpdate = 1;\
	supla_esp_cfg_save(&supla_esp_cfg);\
	os_delay_us(200); \
	supla_system_restart(); };\
	}; 

char* ICACHE_FLASH_ATTR supla_esp_board_cfg_html_template(
    char dev_name[25], const char mac[6], const char data_saved);
void ICACHE_FLASH_ATTR
supla_esp_board_send_channel_values_with_delay(void* srpc);

#define BOARD_ON_INPUT_ACTIVE                        \
    supla_esp_board_gpio_on_input_active(input_cfg); \
    return;
void ICACHE_FLASH_ATTR supla_esp_board_gpio_on_input_active(void* _input_cfg);

#define BOARD_ON_INPUT_INACTIVE                        \
    supla_esp_board_gpio_on_input_inactive(input_cfg); \
    return;
void ICACHE_FLASH_ATTR supla_esp_board_gpio_on_input_inactive(void* _input_cfg);
#endif
