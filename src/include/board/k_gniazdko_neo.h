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

#ifndef K_GNIAZDKO_NEO_H_
#define K_GNIAZDKO_NEO_H_

#define ESP8266_SUPLA_PROTO_VERSION 12

#define BOARD_ON_CONNECT

#define SUPLA_ESP_SOFTVER "2.8.12.0"

#define LED_RED_PORT     4
#define B_RELAY1_PORT    12
#define B_CFG_PORT       13
#define B_UPD_PORT		 20

#define AP_SSID "GNIAZDKO_NEO"
#define ESP_HOSTNAME "GNIAZDKO_NEO"
#define CFGMODE_SSID_LIMIT_MACLEN

#define BOARD_GPIO_OUTPUT_SET_HI if (supla_last_state == STATE_CONNECTED) \
	{if (port == LED_RED_PORT) { hi =!supla_esp_gpio_output_is_hi(B_RELAY1_PORT);\
	 } else if (port==B_RELAY1_PORT) {\
		supla_esp_gpio_set_led(hi, 1, 1); }\
	  else if (port == 20) { \
		supla_esp_board_gpiooutput_set_hi(port, hi); 	\
		return; } };

#define BOARD_ON_CHANNEL_STATE_PREPARE	state->Fields |= SUPLA_CHANNELSTATE_FIELD_LASTCONNECTIONRESETCAUSE;	\
										state->LastConnectionResetCause = supla_esp_cfg.UpdateStatus;		

void supla_esp_board_gpiooutput_set_hi(uint8 port, uint8 hi);

void supla_esp_board_send_channel_values_with_delay(void *srpc);

void ICACHE_FLASH_ATTR supla_esp_board_on_connect(void);

void supla_esp_board_send_channel_values_with_delay(void *srpc);

#define BOARD_ON_INPUT_ACTIVE                        \
    supla_esp_board_gpio_on_input_active(input_cfg); \
    return;
void ICACHE_FLASH_ATTR supla_esp_board_gpio_on_input_active(void* _input_cfg);

#define BOARD_ON_INPUT_INACTIVE                        \
    supla_esp_board_gpio_on_input_inactive(input_cfg); \
    return;
void ICACHE_FLASH_ATTR supla_esp_board_gpio_on_input_inactive(void* _input_cfg);

#endif
