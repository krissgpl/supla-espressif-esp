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

// moj klucz
#include "public_key_in_c_code"

#include "supla_esp.h"
#include "supla_esp_gpio.h"
#include "supla_esp_input.h"

ETSTimer value_timer1;

uint8 UPD_CH;

void ICACHE_FLASH_ATTR supla_esp_board_set_device_name(char *buffer, uint8 buffer_size) {
	
		ets_snprintf(buffer, buffer_size, "SUPLA-VERSA");
	
}

void supla_esp_baord_value_timer1_cb(void *timer_arg) {
	
	supla_log(LOG_DEBUG, "TIMER update - restart");
	supla_system_restart();
	
}

void ICACHE_FLASH_ATTR supla_esp_board_gpio_init(void) {
		
	supla_input_cfg[0].type = INPUT_TYPE_BTN_MONOSTABLE;
	supla_input_cfg[0].gpio_id = B_CFG_PORT;
	supla_input_cfg[0].flags = INPUT_FLAG_PULLUP | INPUT_FLAG_CFG_BTN;
	
	supla_input_cfg[1].type = INPUT_TYPE_SENSOR;
	supla_input_cfg[1].gpio_id = B_SENSOR_PORT1;
	supla_input_cfg[1].channel = 0;
	
	supla_input_cfg[2].type = INPUT_TYPE_SENSOR;
	supla_input_cfg[2].gpio_id = B_SENSOR_PORT2;
	supla_input_cfg[2].channel = 1;
	
	supla_input_cfg[3].type = INPUT_TYPE_SENSOR;
	supla_input_cfg[3].gpio_id = B_SENSOR_PORT3;
	supla_input_cfg[3].channel = 2;
	
	supla_input_cfg[4].type = INPUT_TYPE_SENSOR;
	supla_input_cfg[4].gpio_id = B_SENSOR_PORT4;
	supla_input_cfg[4].channel = 3;
	
	supla_input_cfg[5].type = INPUT_TYPE_SENSOR;
	supla_input_cfg[5].gpio_id = B_SENSOR_PORT5;
	supla_input_cfg[5].channel = 4;
	
	supla_input_cfg[6].type = INPUT_TYPE_SENSOR;
	supla_input_cfg[6].gpio_id = B_SENSOR_PORT6;
	supla_input_cfg[6].channel = 5;
	
	//----------------------------------------
	
    supla_relay_cfg[0].gpio_id = B_UPD_PORT;	// update init channel
    supla_relay_cfg[0].channel = 6;
		
	// ---------------------------------------
	
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_SD_DATA3_U, FUNC_GPIO10);//uzycie GPIO10
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12);	//uzycie GPIO12
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_GPIO13);	//uzycie GPIO13
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, FUNC_GPIO14);	//uzycie GPIO14
	
	PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO0_U);			// pullup gpio 0
	PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO4_U);			// pullup gpio 4
	PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO5_U);			// pullup gpio 5
	PIN_PULLUP_EN(PERIPHS_IO_MUX_SD_DATA3_U);		// pullup gpio 10
	PIN_PULLUP_EN(PERIPHS_IO_MUX_MTDI_U);			// pullup gpio 12
	PIN_PULLUP_EN(PERIPHS_IO_MUX_MTCK_U);			// pullup gpio 13
	PIN_PULLUP_EN(PERIPHS_IO_MUX_MTMS_U);			// pullup gpio 14

}

void ICACHE_FLASH_ATTR supla_esp_board_set_channels(TDS_SuplaDeviceChannel_C *channels, unsigned char *channel_count) {
	

	*channel_count = 8;
	

	channels[0].Number = 0;
	channels[0].Type = SUPLA_CHANNELTYPE_SENSORNO;
	channels[0].FuncList = 0;
	channels[0].Default = 0;
	channels[0].value[0] = 0;

	channels[1].Number = 1;
	channels[1].Type = SUPLA_CHANNELTYPE_SENSORNO;
	channels[1].FuncList = 0;
	channels[1].Default = 0;
	channels[1].value[0] = 0;
	
	channels[2].Number = 2;
	channels[2].Type = SUPLA_CHANNELTYPE_SENSORNO;
	channels[2].FuncList = 0;
	channels[2].Default = 0;
	channels[2].value[0] = 0;
	
	channels[3].Number = 3;
	channels[3].Type = SUPLA_CHANNELTYPE_SENSORNO;
	channels[3].FuncList = 0;
	channels[3].Default = 0;
	channels[3].value[0] = 0;

	channels[4].Number = 4;
	channels[4].Type = SUPLA_CHANNELTYPE_SENSORNO;
	channels[4].FuncList = 0;
	channels[4].Default = 0;
	channels[4].value[0] = 0;

	channels[5].Number = 5;
	channels[5].Type = SUPLA_CHANNELTYPE_SENSORNO;
	channels[5].FuncList = 0;
	channels[5].Default = 0;
	channels[5].value[0] = 0;
	
	channels[6].Number = 6;
	channels[6].Type = SUPLA_CHANNELTYPE_RELAY;
	channels[6].FuncList = SUPLA_BIT_FUNC_POWERSWITCH;
	channels[6].Flags = SUPLA_CHANNEL_FLAG_CHANNELSTATE;
	channels[6].Default = 0;
	channels[6].value[0] = supla_esp_gpio_relay_on(B_UPD_PORT);
	
	channels[7].Number = 7;
	channels[7].Type = SUPLA_CHANNELTYPE_ACTIONTRIGGER;
	channels[7].FuncList = SUPLA_CHANNELFNC_ACTIONTRIGGER;
	//channels[7].Flags = 0;
	channels[7].Default = 0;
	channels[7].value[0] = 0;
	
}

void ICACHE_FLASH_ATTR supla_esp_board_send_channel_values_with_delay(void *srpc) {

	supla_esp_channel_value_changed(0, !gpio__input_get(B_SENSOR_PORT1));
	supla_esp_channel_value_changed(1, !gpio__input_get(B_SENSOR_PORT2));
	supla_esp_channel_value_changed(2, !gpio__input_get(B_SENSOR_PORT3));
	supla_esp_channel_value_changed(3, !gpio__input_get(B_SENSOR_PORT4));
	supla_esp_channel_value_changed(4, !gpio__input_get(B_SENSOR_PORT5));
	supla_esp_channel_value_changed(5, !gpio__input_get(B_SENSOR_PORT6));	
	supla_esp_channel_value_changed(6, supla_esp_gpio_relay_on(B_UPD_PORT));

}
/*
void ICACHE_FLASH_ATTR supla_esp_board_gpio_relay_switch(void* _input_cfg,
    char hi)
{

    supla_input_cfg_t* input_cfg = (supla_input_cfg_t*)_input_cfg;

    if (input_cfg->relay_gpio_id != 255) {

        // supla_log(LOG_DEBUG, "RELAY");

        supla_esp_gpio_relay_hi(input_cfg->relay_gpio_id, hi, 0);

        if (input_cfg->channel != 255)
            supla_esp_channel_value_changed(
                input_cfg->channel,
                supla_esp_gpio_relay_is_hi(input_cfg->relay_gpio_id));
    }
}
*/
void ICACHE_FLASH_ATTR supla_esp_board_gpio_on_input_active(void* _input_cfg) {

    supla_input_cfg_t* input_cfg = (supla_input_cfg_t*)_input_cfg;

	supla_log(LOG_DEBUG, "board active");
/*
  bool advanced_mode = supla_esp_input_is_advanced_mode_enabled(input_cfg);

  if (((input_cfg->type == INPUT_TYPE_BTN_MONOSTABLE &&
        input_cfg->flags & INPUT_FLAG_TRIGGER_ON_PRESS) ||
      input_cfg->type == INPUT_TYPE_BTN_BISTABLE ||
      input_cfg->type == INPUT_TYPE_MOTION_SENSOR ||
      advanced_mode) &&
      input_cfg->relay_gpio_id != 255) {
	supla_log(LOG_DEBUG, "INPUT_FLAG_TRIGGER_ON_PRESS active");
 	  
      //unsigned char newState = 255;
      if (input_cfg->type == INPUT_TYPE_MOTION_SENSOR) {
        if (input_cfg->active_triggers & SUPLA_ACTION_CAP_TURN_ON) {
          // ignore when type is motion sensor and AT is configured for turn on
          return;
        }
        //newState = 1;
      
      //supla_esp_gpio_relay_switch_by_input(input_cfg, newState);
    } 
  } else */
	  if (input_cfg->type == INPUT_TYPE_SENSOR && input_cfg->channel != 255) {

    // TODO: add MQTT support for sensor
    supla_esp_channel_value_changed(input_cfg->channel, 0);
  }
}

void ICACHE_FLASH_ATTR
supla_esp_board_gpio_on_input_inactive(void* _input_cfg) {

  supla_input_cfg_t* input_cfg = (supla_input_cfg_t*)_input_cfg;

  supla_log(LOG_DEBUG, "board inactive");
/*
  if (((input_cfg->type == INPUT_TYPE_BTN_MONOSTABLE &&
        !(input_cfg->flags & INPUT_FLAG_TRIGGER_ON_PRESS)) ||
      input_cfg->type == INPUT_TYPE_BTN_BISTABLE ||
      input_cfg->type == INPUT_TYPE_MOTION_SENSOR) &&
      input_cfg->relay_gpio_id != 255) {
	supla_log(LOG_DEBUG, "!INPUT_FLAG_TRIGGER_ON_PRESS inactive");

      //unsigned char newState = 255;
      if (input_cfg->type == INPUT_TYPE_MOTION_SENSOR) {
        if (input_cfg->active_triggers & SUPLA_ACTION_CAP_TURN_OFF) {
          // ignore when type is motion sensor and AT is configured for turn off
          return;
        }
        //newState = 0;
      }
      //supla_esp_gpio_relay_switch_by_input(input_cfg, newState);
    
  } else */
	 if (input_cfg->type == INPUT_TYPE_SENSOR &&
      input_cfg->channel != 255) {

    // TODO: add MQTT support for sensor
    supla_esp_channel_value_changed(input_cfg->channel, 1);
  }
}

void ICACHE_FLASH_ATTR supla_esp_board_gpiooutput_set_hi(int port, char hi) {
	
	supla_log(LOG_DEBUG, "supla_esp_board_gpiooutput_set_hi %i", port);
		
	UPD_CH  = 6;
	
	if ( port == 20 ) {	

		if ( hi == 1 ) {
	
			supla_log(LOG_DEBUG, "update, port = %i", port);
		
			if ( supla_esp_cfg.FirmwareUpdate == 1 ) {
			
				supla_esp_state.Relay[UPD_CH] = 1;
				supla_log(LOG_DEBUG, "value_changed upd - 1");
				supla_esp_save_state(SAVE_STATE_DELAY);
				supla_esp_channel_value_changed(UPD_CH, supla_esp_state.Relay[UPD_CH]);
				os_timer_disarm(&value_timer1);
				os_timer_setfn(&value_timer1, (os_timer_func_t *)supla_esp_baord_value_timer1_cb, NULL);
				os_timer_arm(&value_timer1, 4000, 0);
			};
		
			if ( supla_esp_cfg.FirmwareUpdate == 0 ) {
			
				supla_esp_cfg.FirmwareUpdate = 1; 
				supla_esp_cfg_save(&supla_esp_cfg);
				supla_esp_channel_value_changed(UPD_CH, 1);
				supla_log(LOG_DEBUG, "value_changed upd - 0");
			};
		}; 
	};
}