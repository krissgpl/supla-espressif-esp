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

#ifndef K_SOCKET_H_
#define K_SOCKET_H_

#define ESP8266_SUPLA_PROTO_VERSION 16

#define SUPLA_ESP_SOFTVER "2.8.49.0"

#define BOARD_CFG_HTML_TEMPLATE
#define BOARD_ON_CONNECT
#define CFGMODE_SSID_LIMIT_MACLEN

#ifdef __BOARD_k_socket_ds18b20
	#define DS18B20
	#define TEMPERATURE_CHANNEL 2
	#define AP_SSID "SOCKET_DS18B20"

    	#define W1_GPIO5

#endif

#ifdef __BOARD_k_socket_DHT22
	#define DHTSENSOR
   	#define TEMPERATURE_HUMIDITY_CHANNEL 2
	#define AP_SSID "SOCKET_DHT22"

        #define W1_GPIO5

#endif

#ifdef __BOARD_k_socket
	#define AP_SSID "SOCKET"
#endif

#define LED_RED_PORT      2
#define B_RELAY1_PORT     4
#define B_CFG_PORT        0
#define B_UPD_PORT		  20

#define BOARD_GPIO_OUTPUT_SET_HI if ( port >= 20 ) { supla_esp_board_gpiooutput_set_hi(port, hi); return; };

#define BOARD_ON_CHANNEL_STATE_PREPARE	if ( ChannelNumber == 1 ) {	\
											state->Fields |= SUPLA_CHANNELSTATE_FIELD_LASTCONNECTIONRESETCAUSE;	\
											state->LastConnectionResetCause = supla_esp_cfg.UpdateStatus;	\
										    state->IPv4 = ipaddr_addr(SUPLA_ESP_SOFTVER);	\
											supla_log(LOG_DEBUG, "IP FIELD = %i", ipaddr_addr(SUPLA_ESP_SOFTVER)); };

void supla_esp_board_gpiooutput_set_hi(uint8 port, uint8 hi);


char *ICACHE_FLASH_ATTR supla_esp_board_cfg_html_template(
    char dev_name[25], const char mac[6], const char data_saved);
	
void ICACHE_FLASH_ATTR supla_esp_board_on_connect(void);

void supla_esp_board_send_channel_values_with_delay(void *srpc);

#endif
