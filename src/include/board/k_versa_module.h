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

#ifndef K_VERSA_MODULE_H_
#define K_VERSA_MODULE_H_

#define ESP8266_SUPLA_PROTO_VERSION 16

#define SUPLA_ESP_SOFTVER "2.8.51.0"

//#define BOARD_CFG_HTML_TEMPLATE

#define SEND_BUFFER_SIZE 600

#define ESP_HOSTNAME "SUPLA-VERSA"
#define AP_SSID "SUPLA-VERSA"
#define CFGMODE_SSID_LIMIT_MACLEN

#define USE_GPIO16_OUTPUT

#define B_CFG_PORT          0
#define LED_RED_PORT  		16

#define B_SENSOR_PORT1      14	//ch7 CAP_TOGGLE_x1
#define B_SENSOR_PORT2      12	//ch7 CAP_TOGGLE_x2
#define B_SENSOR_PORT3      13	//ch7 CAP_TOGGLE_x3
#define B_SENSOR_PORT4      5	//ch8 CAP_TOGGLE_x1
#define B_SENSOR_PORT5      4	//ch8 CAP_TOGGLE_x2
#define B_SENSOR_PORT6      10	//ch8 CAP_TOGGLE_x3

#define B_UPD_PORT		    20

#define BOARD_GPIO_OUTPUT_SET_HI	\
		if (port >= 20) {supla_esp_board_gpiooutput_set_hi(port, hi); 	\
						supla_log(LOG_DEBUG, "PORT 20 MAKRO");	\
						return;  };

#define BOARD_ON_CHANNEL_STATE_PREPARE	if ( ChannelNumber == 6 ) {	\
											state->Fields |= SUPLA_CHANNELSTATE_FIELD_LASTCONNECTIONRESETCAUSE;	\
											state->LastConnectionResetCause = supla_esp_cfg.UpdateStatus;	\
										    state->IPv4 = ipaddr_addr(SUPLA_ESP_SOFTVER);	\
											supla_log(LOG_DEBUG, "IP FIELD = %i", ipaddr_addr(SUPLA_ESP_SOFTVER)); };

#define BOARD_INTR_HANDLER	if ( supla_last_state == STATE_CONNECTED && gpio_status > 1 ) { \
							supla_log(LOG_DEBUG, "INTR gpio_status = %i", gpio_status);	\
							supla_board_input(!gpio__input_get(B_SENSOR_PORT1), !gpio__input_get(B_SENSOR_PORT2), !gpio__input_get(B_SENSOR_PORT3),	\
												!gpio__input_get(B_SENSOR_PORT4), !gpio__input_get(B_SENSOR_PORT5), !gpio__input_get(B_SENSOR_PORT6)); }
							
void supla_board_input(char in1, char in2, char in3, char in4, char in5, char in6);

void ICACHE_FLASH_ATTR supla_esp_board_gpiooutput_set_hi(int port, char hi);

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