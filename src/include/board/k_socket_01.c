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

#include "public_key_in_c_code" 

#include "supla_BME280"

void supla_esp_board_set_device_name(char *buffer, uint8 buffer_size) {
		ets_snprintf(buffer, buffer_size, "SOCKET-01");
}


void supla_esp_board_gpio_init(void) {
		
	supla_input_cfg[0].type = INPUT_TYPE_BTN_MONOSTABLE;
	supla_input_cfg[0].gpio_id = B_CFG_PORT;
	supla_input_cfg[0].flags = INPUT_FLAG_PULLUP | INPUT_FLAG_CFG_BTN;
	supla_input_cfg[0].relay_gpio_id = B_RELAY1_PORT;
	supla_input_cfg[0].channel = 0;

	// ---------------------------------------
	
	supla_input_cfg[1].type = INPUT_TYPE_BTN_MONOSTABLE;	//jest potrzebne zeby gpio0 dzialalo
	supla_input_cfg[1].gpio_id = 5;
	
	// ---------------------------------------

    supla_relay_cfg[0].gpio_id = B_RELAY1_PORT;
    supla_relay_cfg[0].flags = RELAY_FLAG_RESET;
    supla_relay_cfg[0].channel = 0;
	
	// ---------------------------------------
	
	PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO0_U);	// pullup gpio 0
	
	i2c_master_gpio_init();

}

void supla_esp_board_set_channels(TDS_SuplaDeviceChannel_C *channels, unsigned char *channel_count) {	

    *channel_count = 1;

	channels[0].Number = 0;
	channels[0].Type = SUPLA_CHANNELTYPE_RELAY;
	channels[0].FuncList = SUPLA_BIT_FUNC_POWERSWITCH \
								| SUPLA_BIT_FUNC_LIGHTSWITCH \
								| SUPLA_BIT_FUNC_CONTROLLINGTHEGATEWAYLOCK \
								| SUPLA_BIT_FUNC_CONTROLLINGTHEGATE \
								| SUPLA_BIT_FUNC_CONTROLLINGTHEGARAGEDOOR \
								| SUPLA_BIT_FUNC_CONTROLLINGTHEDOORLOCK;
	channels[0].Flags = SUPLA_CHANNEL_FLAG_CHANNELSTATE;							
	channels[0].Default = SUPLA_CHANNELFNC_POWERSWITCH;
	channels[0].value[0] = supla_esp_gpio_relay_on(B_RELAY1_PORT);

}



void supla_esp_board_send_channel_values_with_delay(void *srpc) {

	supla_esp_channel_value_changed(0, supla_esp_gpio_relay_on(B_RELAY1_PORT));
	
}
