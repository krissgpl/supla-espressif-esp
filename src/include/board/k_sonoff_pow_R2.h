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

#ifndef K_SONOFF_POW_R2_H_
#define K_SONOFF_POW_R2_H_
/*
#define GPIO_PORT_INIT \
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0); \
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4); \
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5); \
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12); \
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_GPIO13); \
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, FUNC_GPIO14); \
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDO_U, FUNC_GPIO15)
#endif
*/
#include "supla_esp.h"
#include "supla-dev/proto.h"
#include <sntp.h>

#define ESP8266_SUPLA_PROTO_VERSION 16

#define SUPLA_ESP_SOFTVER "2.8.51.0"

#define ELECTRICITIMETER
#define ELECTRICITY_METER_COUNT 1
#define ELECTRICITY_METER_CHANNEL_OFFSET 0

#define BOARD_CFG_HTML_TEMPLATE
#define AP_SSID "SONOFF_POW_R2"
#define BOARD_ON_CONNECT

#define B_RELAY1_PORT    12
#define B_CFG_PORT        0
#define LED_RED_PORT     13

/*#define BOARD_GPIO_OUTPUT_SET_HI if (supla_last_state == STATE_CONNECTED) {if (port == 20) { \
 	supla_log(LOG_DEBUG, "update, port = %i", port); \
	supla_esp_cfg.FirmwareUpdate = 1;\
	supla_esp_cfg_save(&supla_esp_cfg);\
	supla_esp_devconn_system_restart(); };  }; */

char *ICACHE_FLASH_ATTR supla_esp_board_cfg_html_template(
    char dev_name[25], const char mac[6], const char data_saved);

//void ICACHE_FLASH_ATTR supla_esp_em_get_value(
//    unsigned char channel_number, char value[SUPLA_CHANNELVALUE_SIZE]);

//extern ETSTimer supla_pow_timer1;
//int status_ok;

int ICACHE_FLASH_ATTR supla_esp_board_get_measurements(unsigned char channel_number, TElectricityMeter_ExtendedValue_V2 *pow_ev);

void supla_esp_board_send_channel_values_with_delay(void *srpc);

void ICACHE_FLASH_ATTR supla_esp_board_on_connect(void);

#endif