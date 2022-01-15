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
 TEST
 */

#include "public_key_in_c_code"
 
#include "k_dimmer.h"
#include "supla_esp_devconn.h"

int HRM_channel;
int UPD_channel;
int BLK_channel;
int BL1_channel;
int BL2_channel;

uint8 dimmer_brightness[2] = {0, 0};

uint8 Licznik = 0;
uint8 Jasnosc = 0;
int Czas;

unsigned int Wlacznik1 = 0;
unsigned int Wlacznik2 = 0;

ETSTimer dimmer_timer;
ETSTimer value_timer1;
ETSTimer work_timer;

void ICACHE_FLASH_ATTR supla_esp_board_set_device_name(char *buffer, uint8 buffer_size) {
	ets_snprintf(buffer, buffer_size, "STAIRCASE-DIMMER");
}


void ICACHE_FLASH_ATTR supla_esp_board_gpio_init(void) {
		
	supla_input_cfg[0].type = INPUT_TYPE_BTN_MONOSTABLE;
	supla_input_cfg[0].gpio_id = B_CFG_PORT;
	supla_input_cfg[0].flags = INPUT_FLAG_PULLUP | INPUT_FLAG_CFG_BTN;
	
	supla_input_cfg[1].type = INPUT_TYPE_SENSOR;
	supla_input_cfg[1].gpio_id = B_SENSOR_PORT1;
	supla_input_cfg[1].channel = 2;
	
	supla_input_cfg[2].type = INPUT_TYPE_SENSOR;
	supla_input_cfg[2].gpio_id = B_SENSOR_PORT2;
	supla_input_cfg[2].channel = 3;
	
	supla_input_cfg[3].type = INPUT_TYPE_BTN_MONOSTABLE;
    supla_input_cfg[3].gpio_id = 4;
	
	supla_relay_cfg[0].gpio_id = B_HARMONOGRAM;	// harmonogram channel
    supla_relay_cfg[0].channel = 4;
	
	supla_relay_cfg[1].gpio_id = B_SWITCH;		// wlacznik tasmy
    supla_relay_cfg[1].channel = 5;
	
	supla_relay_cfg[2].gpio_id = B_UPD_PORT;	// update init channel
	supla_relay_cfg[2].flags = RELAY_FLAG_RESET;
    supla_relay_cfg[2].channel = 6;
	
	supla_relay_cfg[3].gpio_id = B_SENSOR_BLOCK1;		// wlacznik wejscia 1
    supla_relay_cfg[3].channel = 7;
	
	supla_relay_cfg[4].gpio_id = B_SENSOR_BLOCK2;		// wlacznik wejscia 2
    supla_relay_cfg[4].channel = 8;

	
	// ---------------------------------------
	
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_SD_DATA3_U, FUNC_GPIO10);//uzycie GPIO10
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12);	//uzycie GPIO12
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, FUNC_GPIO14);	//uzycie GPIO14
	
	PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO0_U);			// pullup gpio 0
	PIN_PULLUP_EN(PERIPHS_IO_MUX_SD_DATA3_U);		// pullup gpio 10
	PIN_PULLUP_EN(PERIPHS_IO_MUX_MTDI_U);			// pullup gpio 12
	PIN_PULLUP_EN(PERIPHS_IO_MUX_MTMS_U);			// pullup gpio 14
	
}

void supla_esp_baord_value_timer1_cb(void *timer_arg) {
	
	supla_log(LOG_DEBUG, "TIMER update - restart");
	supla_system_restart();
	
}

void dimmer_timer_ON_cb(void *timer_arg) {
	
	supla_log(LOG_DEBUG, "Dimmer Timer ON start");
	
	Licznik ++;
	
	supla_log(LOG_DEBUG, "Licznik : %i", Licznik);
	supla_esp_pwm_set_percent_duty(Licznik, 100, 0);
	
	if ( supla_esp_gpio_output_is_hi(B_HARMONOGRAM) == 1 ) { Jasnosc = 100;
	} else { Jasnosc = supla_esp_state.brightness[0]; };
	
	 if ( Licznik == Jasnosc ) { 
	 supla_log(LOG_DEBUG, "Dimmer Timer ON stop");
	 os_timer_disarm(&dimmer_timer); }
	
}

void dimmer_timer_OFF_cb(void *timer_arg) {
	
	supla_log(LOG_DEBUG, "Dimmer Timer OFF start");
	
	Licznik --;
	supla_log(LOG_DEBUG, "Licznik : %i", Licznik);
	supla_esp_pwm_set_percent_duty(Licznik, 100, 0);
	
	 if ( Licznik == 0 ) { 
	 supla_log(LOG_DEBUG, "Dimmer Timer OFF stop");
	 Wlacznik1 = 0;
	 Wlacznik2 = 0;
	 os_timer_disarm(&dimmer_timer); }
	
}

void work_timer_cb(void *timer_arg) {
	
	supla_log(LOG_DEBUG, "Set dimmer 0");
	
	if ( supla_esp_gpio_output_is_hi(B_HARMONOGRAM) == 1 ) { Licznik = 100;
	} else { Licznik = supla_esp_state.brightness[0]; };
	
	Wlacznik1 = 0;	// testowo
	Wlacznik2 = 0;	// testowo
	
/*	os_timer_disarm(&dimmer_timer);
	os_timer_setfn(&dimmer_timer, (os_timer_func_t *)dimmer_timer_OFF_cb, NULL);
	os_timer_arm(&dimmer_timer, 20, 1); */
}

void supla_dimmer_smooth(int in1, int in2) {
	
	supla_log(LOG_DEBUG, "dimmer_smooth CH1 = %i, CH2 = %i", in1, in2);
	supla_log(LOG_DEBUG, "dimmer_smooth B_SWITCH = %i", supla_esp_gpio_output_is_hi(B_SWITCH));
	supla_log(LOG_DEBUG, "dimmer_smooth FirmwareUpdate = %i", supla_esp_cfg.FirmwareUpdate);
	
	if ( supla_esp_gpio_output_is_hi(B_SWITCH) == 1 && supla_esp_cfg.FirmwareUpdate == 0 ) {
		
		Czas = supla_esp_state.brightness[1] * 1000;
		supla_log(LOG_DEBUG, "Czas = %i ms", Czas);
		supla_log(LOG_DEBUG, "Wlacznik1 = %i", Wlacznik1);
		supla_log(LOG_DEBUG, "Wlacznik2 = %i", Wlacznik2);
		
		if ( supla_esp_gpio_output_is_hi(B_SENSOR_BLOCK1) == 1 ) {
		
			if ( Wlacznik1 == 0 && in1 == 1 ) { 
				supla_log(LOG_DEBUG, "Set dimmer 1 in1=1");
				Licznik = 0;
				Wlacznik1 = 1;
			/*	os_timer_disarm(&dimmer_timer);
				os_timer_setfn(&dimmer_timer, (os_timer_func_t *)dimmer_timer_ON_cb, NULL);
				os_timer_arm(&dimmer_timer, 20, 1); */
			};
		
			if ( Wlacznik1 == 1 && in1 == 0 && supla_esp_gpio_output_is_hi(B_SENSOR_BLOCK2) == 1) {
				
				if ( supla_esp_gpio_output_is_hi(B_SENSOR_BLOCK2) == 0) {
					os_timer_disarm(&work_timer);
					os_timer_setfn(&work_timer, (os_timer_func_t *)work_timer_cb, NULL);
					os_timer_arm(&work_timer, Czas, 0); };
				
				if { supla_esp_gpio_output_is_hi(B_SENSOR_BLOCK2) == 1 && in2 == 0 ) {
					os_timer_disarm(&work_timer);
					os_timer_setfn(&work_timer, (os_timer_func_t *)work_timer_cb, NULL);
					os_timer_arm(&work_timer, Czas, 0); };
			};
		};
	
		if ( supla_esp_gpio_output_is_hi(B_SENSOR_BLOCK2) == 1 ) {
		
			if ( Wlacznik2 == 0 && in2 == 1 ) { 
				supla_log(LOG_DEBUG, "Set dimmer 2 in2=1");
				Licznik = 0;
				Wlacznik2 = 1;
			/*	os_timer_disarm(&dimmer_timer);
				os_timer_setfn(&dimmer_timer, (os_timer_func_t *)dimmer_timer_ON_cb, NULL);
				os_timer_arm(&dimmer_timer, 20, 1); */
			};

			if ( Wlacznik2 == 1 && in2 == 0 && supla_esp_gpio_output_is_hi(B_SENSOR_BLOCK2) == 1) {
				
				if ( supla_esp_gpio_output_is_hi(B_SENSOR_BLOCK1) == 0) {
					os_timer_disarm(&work_timer);
					os_timer_setfn(&work_timer, (os_timer_func_t *)work_timer_cb, NULL);
					os_timer_arm(&work_timer, Czas, 0); };
				
				if { supla_esp_gpio_output_is_hi(B_SENSOR_BLOCK1) == 1 && in1 == 0 ) {
					os_timer_disarm(&work_timer);
					os_timer_setfn(&work_timer, (os_timer_func_t *)work_timer_cb, NULL);
					os_timer_arm(&work_timer, Czas, 0); };
			};

		};
	};
		
		
/*if ( supla_esp_gpio_output_is_hi(B_SWITCH) == 1 && supla_esp_cfg.FirmwareUpdate == 0 ) {
	
	Czas = supla_esp_state.brightness[1] * 1000;
	Wlacznik = Wlacznik + 1;
	supla_log(LOG_DEBUG, "Wlacznik = %i", Wlacznik);
	supla_log(LOG_DEBUG, "Hi = %i", hi);
	supla_log(LOG_DEBUG, "Czas = %i ms", Czas);
	
	if ( Wlacznik == 2 && hi == 1 ) { supla_log(LOG_DEBUG, "Set dimmer 1 hi=1");
					Licznik = 0;
					os_timer_disarm(&dimmer_timer);
					os_timer_setfn(&dimmer_timer, (os_timer_func_t *)dimmer_timer_ON_cb, NULL);
					os_timer_arm(&dimmer_timer, 20, 1);
					};
	
	if ( hi == 2 && Start == 0 ) { supla_log(LOG_DEBUG, "Set dimmer 1 hi=2");
					Licznik = 0;
					Start = 1;
					os_timer_disarm(&dimmer_timer);
					os_timer_setfn(&dimmer_timer, (os_timer_func_t *)dimmer_timer_ON_cb, NULL);
					os_timer_arm(&dimmer_timer, 20, 1);
					}; 

	if ( hi == 0 ) { 
					os_timer_disarm(&work_timer);
					os_timer_setfn(&work_timer, (os_timer_func_t *)work_timer_cb, NULL);
					os_timer_arm(&work_timer, Czas, 0); };
}; */
}

void ICACHE_FLASH_ATTR supla_esp_board_pwm_init(void) {
	//supla_esp_channel_set_rgbw_value(0, 0, 0, supla_esp_state.brightness[0], 0, 0);
	Jasnosc = supla_esp_state.brightness[0];
	supla_log(LOG_DEBUG, "Jasnosc init = %i", Jasnosc);
	supla_log(LOG_DEBUG, "Czas init = %i", supla_esp_state.brightness[1]);
}

void ICACHE_FLASH_ATTR supla_esp_board_set_channels(TDS_SuplaDeviceChannel_C *channels, unsigned char *channel_count) {

	*channel_count = 9;

	channels[0].Type = SUPLA_CHANNELTYPE_DIMMER;
	channels[0].Number = 0;
	channels[0].Flags = SUPLA_CHANNEL_FLAG_CHANNELSTATE;
	supla_esp_channel_rgbw_to_value(channels[0].value, 0, 0, supla_esp_state.brightness[0]);
	
	channels[1].Type = SUPLA_CHANNELTYPE_DIMMER;
	channels[1].Number = 1;
	channels[1].Flags = SUPLA_CHANNEL_FLAG_CHANNELSTATE;
	supla_esp_channel_rgbw_to_value(channels[1].value, 0, 0, supla_esp_state.brightness[1]);
	
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
	channels[4].Type = SUPLA_CHANNELTYPE_RELAY;
	channels[4].FuncList = SUPLA_BIT_FUNC_POWERSWITCH;
	channels[4].Flags = SUPLA_CHANNEL_FLAG_CHANNELSTATE;
	channels[4].Default = SUPLA_CHANNELFNC_POWERSWITCH;
	channels[4].value[0] = supla_esp_gpio_relay_on(B_HARMONOGRAM);
	
	channels[5].Number = 5;
	channels[5].Type = SUPLA_CHANNELTYPE_RELAY;
	channels[5].FuncList = SUPLA_BIT_FUNC_POWERSWITCH;
	channels[5].Flags = SUPLA_CHANNEL_FLAG_CHANNELSTATE;
	channels[5].Default = 0;
	channels[5].value[0] = supla_esp_gpio_relay_on(B_SWITCH);
	
	channels[6].Number = 6;
	channels[6].Type = SUPLA_CHANNELTYPE_RELAY;
	channels[6].FuncList = SUPLA_BIT_FUNC_POWERSWITCH;
	channels[6].Flags = SUPLA_CHANNEL_FLAG_CHANNELSTATE;
	channels[6].Default = 0;
	channels[6].value[0] = supla_esp_gpio_relay_on(B_UPD_PORT);
	
	channels[7].Number = 7;
	channels[7].Type = SUPLA_CHANNELTYPE_RELAY;
	channels[7].FuncList = SUPLA_BIT_FUNC_POWERSWITCH;
	channels[7].Flags = SUPLA_CHANNEL_FLAG_CHANNELSTATE;
	channels[7].Default = 0;
	channels[7].value[0] = supla_esp_gpio_relay_on(B_SENSOR_BLOCK1);
	
	channels[8].Number = 8;
	channels[8].Type = SUPLA_CHANNELTYPE_RELAY;
	channels[8].FuncList = SUPLA_BIT_FUNC_POWERSWITCH;
	channels[8].Flags = SUPLA_CHANNEL_FLAG_CHANNELSTATE;
	channels[8].Default = 0;
	channels[8].value[0] = supla_esp_gpio_relay_on(B_SENSOR_BLOCK2);
}

char ICACHE_FLASH_ATTR supla_esp_board_set_rgbw_value(int ChannelNumber, int *Color, float *ColorBrightness, float *Brightness ) {

	uint8 n = 0;
	
	switch(ChannelNumber) {
	case 0:
		n = 0; break;
	case 1:
		n = 1; break;
	default:
		return 0;
	}
	
	dimmer_brightness[n] = *Brightness;
	
	if ( dimmer_brightness[n] > 100 )
		dimmer_brightness[n] = 100;
	
	if ( ChannelNumber == 0 ) {
		//supla_esp_pwm_set_percent_duty(dimmer_brightness[n], 100, 0);
		supla_log(LOG_DEBUG, "Set dimmer 0 : %i", dimmer_brightness[n]);
	} else if ( ChannelNumber == 1 ) {
		//supla_esp_pwm_set_percent_duty(dimmer_brightness[n], 100, 1);
		supla_log(LOG_DEBUG, "Set dimmer 1 : %i", dimmer_brightness[n]);
	}
	
	return 1; 
}

char *ICACHE_FLASH_ATTR supla_esp_board_cfg_html_template(
    char dev_name[25], const char mac[6], const char data_saved) {
		
  static char html_template_header[] =
      "<!DOCTYPE html><meta http-equiv=\"content-type\" content=\"text/html; "
      "charset=UTF-8\"><meta name=\"viewport\" "
      "content=\"width=device-width,initial-scale=1,maximum-scale=1,user-"
      "scalable=no\"><style>body{font-size:14px;font-family:HelveticaNeue,"
      "\"Helvetica Neue\",HelveticaNeueRoman,HelveticaNeue-Roman,\"Helvetica "
      "Neue "
      "Roman\",TeXGyreHerosRegular,Helvetica,Tahoma,Geneva,Arial,sans-serif;"
      "font-weight:400;font-stretch:normal;background:#00d151;color:#fff;line-"
      "height:20px;padding:0}.s{width:460px;margin:0 auto;margin-top:calc(50vh "
      "- 340px);border:solid 3px #fff;padding:0 10px "
      "10px;border-radius:3px}#l{display:block;max-width:150px;height:155px;"
      "margin:-80px auto 20px;background:#00d151;padding-right:5px}#l "
      "path{fill:#000}.w{margin:3px 0 16px;padding:5px "
      "0;border-radius:3px;background:#fff;box-shadow:0 1px 3px "
      "rgba(0,0,0,.3)}h1,h3{margin:10px "
      "8px;font-family:HelveticaNeueLight,HelveticaNeue-Light,\"Helvetica Neue "
      "Light\",HelveticaNeue,\"Helvetica "
      "Neue\",TeXGyreHerosRegular,Helvetica,Tahoma,Geneva,Arial,sans-serif;"
      "font-weight:300;font-stretch:normal;color:#000;font-size:23px}h1{margin-"
      "bottom:14px;color:#fff}span{display:block;margin:10px 7px "
      "14px}i{display:block;font-style:normal;position:relative;border-bottom:"
      "solid 1px "
      "#00d151;height:42px}i:last-child{border:none}label{position:absolute;"
      "display:inline-block;top:0;left:8px;color:#00d151;line-height:41px;"
      "pointer-events:none}input,select{width:calc(100% - "
      "145px);border:none;font-size:16px;line-height:40px;border-radius:0;"
      "letter-spacing:-.5px;background:#fff;color:#000;padding-left:144px;-"
      "webkit-appearance:none;-moz-appearance:none;appearance:none;outline:0!"
      "important;height:40px}select{padding:0;float:right;margin:1px 3px 1px "
      "2px}button{width:100%;border:0;background:#000;padding:5px "
      "10px;font-size:16px;line-height:40px;color:#fff;border-radius:3px;box-"
      "shadow:0 1px 3px "
      "rgba(0,0,0,.3);cursor:pointer}.c{background:#ffe836;position:fixed;"
      "width:100%;line-height:80px;color:#000;top:0;left:0;box-shadow:0 1px "
      "3px rgba(0,0,0,.3);text-align:center;font-size:26px;z-index:100}@media "
      "all and (max-height:920px){.s{margin-top:80px}}@media all and "
      "(max-width:900px){.s{width:calc(100% - "
      "20px);margin-top:40px;border:none;padding:0 "
      "8px;border-radius:0}#l{max-width:80px;height:auto;margin:10px auto "
      "20px}h1,h3{font-size:19px}i{border:none;height:auto}label{display:block;"
      "margin:4px 0 "
      "12px;color:#00d151;font-size:13px;position:relative;line-height:18px}"
      "input,select{width:calc(100% - "
      "10px);font-size:16px;line-height:28px;padding:0 5px;border-bottom:solid "
      "1px #00d151}select{width:100%;float:none;margin:0}}</style><script "
      "type=\"text/javascript\">setTimeout(function(){var element =  "
      "document.getElementById('msg');if ( element != null ) "
      "element.style.visibility = \"hidden\";},3200);</script>";
  
  static char html_template[] =
      "%s%s<div class=\"s\"><svg version=\"1.1\" id=\"l\" x=\"0\" y=\"0\" "
      "viewBox=\"0 0 200 200\" xml:space=\"preserve\"><path "
      "d=\"M59.3,2.5c18.1,0.6,31.8,8,40.2,23.5c3.1,5.7,4.3,11.9,4.1,18.3c-0.1,"
      "3.6-0.7,7.1-1.9,10.6c-0.2,0.7-0.1,1.1,0.6,1.5c12.8,7.7,25.5,15.4,38.3,"
      "23c2.9,1.7,5.8,3.4,8.7,5.3c1,0.6,1.6,0.6,2.5-0.1c4.5-3.6,9.8-5.3,15.7-5."
      "4c12.5-0.1,22.9,7.9,25.2,19c1.9,9.2-2.9,19.2-11.8,23.9c-8.4,4.5-16.9,4."
      "5-25.5,0.2c-0.7-0.3-1-0.2-1.5,0.3c-4.8,4.9-9.7,9.8-14.5,14.6c-5.3,5.3-"
      "10.6,10.7-15.9,16c-1.8,1.8-3.6,3.7-5.4,5.4c-0.7,0.6-0.6,1,0,1.6c3.6,3.4,"
      "5.8,7.5,6.2,12.2c0.7,7.7-2.2,14-8.8,18.5c-12.3,8.6-30.3,3.5-35-10.4c-2."
      "8-8.4,0.6-17.7,8.6-22.8c0.9-0.6,1.1-1,0.8-2c-2-6.2-4.4-12.4-6.6-18.6c-6."
      "3-17.6-12.7-35.1-19-52.7c-0.2-0.7-0.5-1-1.4-0.9c-12.5,0.7-23.6-2.6-33-"
      "10.4c-8-6.6-12.9-15-14.2-25c-1.5-11.5,1.7-21.9,9.6-30.7C32.5,8.9,42.2,4."
      "2,53.7,2.7c0.7-0.1,1.5-0.2,2.2-0.2C57,2.4,58.2,2.5,59.3,2.5z "
      "M76.5,81c0,0.1,0.1,0.3,0.1,0.6c1.6,6.3,3.2,12.6,4.7,18.9c4.5,17.7,8.9,"
      "35.5,13.3,53.2c0.2,0.9,0.6,1.1,1.6,0.9c5.4-1.2,10.7-0.8,15.7,1.6c0.8,0."
      "4,1.2,0.3,1.7-0.4c11.2-12.9,22.5-25.7,33.4-38.7c0.5-0.6,0.4-1,0-1.6c-5."
      "6-7.9-6.1-16.1-1.3-24.5c0.5-0.8,0.3-1.1-0.5-1.6c-9.1-4.7-18.1-9.3-27.2-"
      "14c-6.8-3.5-13.5-7-20.3-10.5c-0.7-0.4-1.1-0.3-1.6,0.4c-1.3,1.8-2.7,3.5-"
      "4.3,5.1c-4.2,4.2-9.1,7.4-14.7,9.7C76.9,80.3,76.4,80.3,76.5,81z "
      "M89,42.6c0.1-2.5-0.4-5.4-1.5-8.1C83,23.1,74.2,16.9,61.7,15.8c-10-0.9-18."
      "6,2.4-25.3,9.7c-8.4,9-9.3,22.4-2.2,32.4c6.8,9.6,19.1,14.2,31.4,11.9C79."
      "2,67.1,89,55.9,89,42.6z "
      "M102.1,188.6c0.6,0.1,1.5-0.1,2.4-0.2c9.5-1.4,15.3-10.9,11.6-19.2c-2.6-5."
      "9-9.4-9.6-16.8-8.6c-8.3,1.2-14.1,8.9-12.4,16.6C88.2,183.9,94.4,188.6,"
      "102.1,188.6z "
      "M167.7,88.5c-1,0-2.1,0.1-3.1,0.3c-9,1.7-14.2,10.6-10.8,18.6c2.9,6.8,11."
      "4,10.3,19,7.8c7.1-2.3,11.1-9.1,9.6-15.9C180.9,93,174.8,88.5,167.7,88."
      "5z\"/></svg><h1>%s</h1><span>LAST STATE: %s<br>Firmware: %s<br>GUID: "
      "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X<br>MAC:"
      " %02X:%02X:%02X:%02X:%02X:%02X</span><form method=\"post\"><div "
      "class=\"w\"><h3>Wi-Fi Settings</h3><i><input name=\"sid\" "
      "value=\"%s\"><label>Network name</label></i><i><input "
      "name=\"wpw\"><label>Password</label></i></div><div "
      "class=\"w\"><h3>Supla Settings</h3><i><input name=\"svr\" "
      "value=\"%s\"><label>Server</label></i><i><input name=\"eml\" "
      "value=\"%s\"><label>E-mail</label></i></div><div "
      "class=\"w\"><h3>Additional Settings</h3>"
	  "<i><select name=\"led\"><option value=\"0\" %s>LED "
      "ON<option value=\"1\" %s>LED OFF<option value=\"2\" %s>CHANNEL STATUS</select><label>Status LED</label></i>"
	  "<i><select name=\"upd\"><option value=\"0\" "
      "%s>NO<option value=\"1\" %s>YES</select><label>Firmware update</label>"
	  "</i></div><button type=\"submit\">SAVE</button><input "
	  "type=\"hidden\" name=\"rbt\" value=\"2\" /></form></div><br><br></body></html>!";

  int bufflen = strlen(supla_esp_get_laststate()) + strlen(dev_name) +
                strlen(SUPLA_ESP_SOFTVER) + strlen(supla_esp_cfg.WIFI_SSID) +
                strlen(supla_esp_cfg.Server) + strlen(supla_esp_cfg.Email) +
                strlen(html_template_header) + strlen(html_template) + 200;

  char *buffer = (char *)malloc(bufflen);

  ets_snprintf(
      buffer, bufflen, html_template, html_template_header,
      data_saved == 1 ? "<div id=\"msg\" class=\"c\">Data saved</div>" : "",
      dev_name, supla_esp_get_laststate(), SUPLA_ESP_SOFTVER,
      (unsigned char)supla_esp_cfg.GUID[0],
      (unsigned char)supla_esp_cfg.GUID[1],
      (unsigned char)supla_esp_cfg.GUID[2],
      (unsigned char)supla_esp_cfg.GUID[3],
      (unsigned char)supla_esp_cfg.GUID[4],
      (unsigned char)supla_esp_cfg.GUID[5],
      (unsigned char)supla_esp_cfg.GUID[6],
      (unsigned char)supla_esp_cfg.GUID[7],
      (unsigned char)supla_esp_cfg.GUID[8],
      (unsigned char)supla_esp_cfg.GUID[9],
      (unsigned char)supla_esp_cfg.GUID[10],
      (unsigned char)supla_esp_cfg.GUID[11],
      (unsigned char)supla_esp_cfg.GUID[12],
      (unsigned char)supla_esp_cfg.GUID[13],
      (unsigned char)supla_esp_cfg.GUID[14],
      (unsigned char)supla_esp_cfg.GUID[15],
      (unsigned char)mac[0],
      (unsigned char)mac[1],
      (unsigned char)mac[2],
      (unsigned char)mac[3],
      (unsigned char)mac[4],
      (unsigned char)mac[5],
      supla_esp_cfg.WIFI_SSID,
      supla_esp_cfg.Server, supla_esp_cfg.Email,
	  supla_esp_cfg.StatusLedOff == 0 ? "selected" : "",
      supla_esp_cfg.StatusLedOff == 1 ? "selected" : "",
	  supla_esp_cfg.StatusLedOff == 2 ? "selected" : "",
	  supla_esp_cfg.FirmwareUpdate == 0 ? "selected" : "",
      supla_esp_cfg.FirmwareUpdate == 1 ? "selected" : ""
      );

  return buffer;
}

void ICACHE_FLASH_ATTR supla_esp_board_on_connect(void) {
  
  if ( supla_esp_cfg.StatusLedOff == 0 || supla_esp_cfg.StatusLedOff == 1 ) {
		supla_esp_gpio_set_led(!supla_esp_cfg.StatusLedOff, 0, 0);
	} else {
		supla_esp_gpio_set_led(supla_esp_gpio_output_is_hi(B_SWITCH), 0, 0);
	}
}


void ICACHE_FLASH_ATTR supla_esp_board_get_rgbw_value(int ChannelNumber, int *Color, float *ColorBrightness, float *Brightness) {

	if ( Brightness != NULL ) {
		if ( ChannelNumber == 0 ) {
			*Brightness = dimmer_brightness[0];
		} else if ( ChannelNumber == 1 ) {
			*Brightness = dimmer_brightness[1];
		}
	}
}

void ICACHE_FLASH_ATTR supla_esp_board_send_channel_values_with_delay(void *srpc) {
	
	supla_esp_channel_value_changed(2, gpio__input_get(B_SENSOR_PORT1));
	supla_esp_channel_value_changed(3, gpio__input_get(B_SENSOR_PORT2));
	supla_esp_channel_value_changed(4, supla_esp_gpio_relay_on(B_HARMONOGRAM));
	supla_esp_channel_value_changed(5, supla_esp_gpio_relay_on(B_SWITCH));
	supla_esp_channel_value_changed(6, supla_esp_gpio_relay_on(B_UPD_PORT));
	supla_esp_channel_value_changed(7, supla_esp_gpio_relay_on(B_SENSOR_BLOCK1));
	supla_esp_channel_value_changed(8, supla_esp_gpio_relay_on(B_SENSOR_BLOCK2));
	
}

void ICACHE_FLASH_ATTR supla_esp_board_gpiooutput_set_hi(uint8 port, uint8 hi) {
			
		supla_log(LOG_DEBUG, "supla_esp_board_gpiooutput_set_hi port = %i, hi = %i", port, hi);
		
		HRM_channel = 4;
		BLK_channel = 5;
		UPD_channel = 6;
		BL1_channel = 7;
		BL2_channel = 8;
				
if ( port == B_UPD_PORT ) {	

	supla_esp_state.Relay[UPD_channel] = hi;
	supla_esp_save_state(SAVE_STATE_DELAY);
	supla_esp_channel_value_changed(UPD_channel, supla_esp_state.Relay[UPD_channel]);
	supla_esp_cfg_save(&supla_esp_cfg);
	supla_esp_channel_value_changed(UPD_channel, hi);

	if ( hi == 1 ) {
	
		supla_log(LOG_DEBUG, "update, port = %i", port);
		
		supla_esp_cfg.FirmwareUpdate = 1; 
		supla_esp_cfg_save(&supla_esp_cfg);

		os_timer_disarm(&value_timer1);
		os_timer_setfn(&value_timer1, (os_timer_func_t *)supla_esp_baord_value_timer1_cb, NULL);
		os_timer_arm(&value_timer1, 4000, 0);
	};
		

}; 

		
if ( port == B_HARMONOGRAM ) {	
			
	supla_esp_state.Relay[HRM_channel] = hi;
	supla_esp_save_state(SAVE_STATE_DELAY);
	supla_esp_channel_value_changed(HRM_channel, supla_esp_state.Relay[HRM_channel]);
	supla_esp_cfg_save(&supla_esp_cfg);
	supla_esp_channel_value_changed(HRM_channel, hi);
				
};
	
if ( port == B_SWITCH ) {	
			
		supla_esp_state.Relay[BLK_channel] = hi;
		supla_esp_save_state(SAVE_STATE_DELAY);
		supla_esp_channel_value_changed(BLK_channel, supla_esp_state.Relay[BLK_channel]);
		supla_esp_cfg_save(&supla_esp_cfg);
		supla_esp_channel_value_changed(BLK_channel, hi);
	}
	
if ( port == B_SENSOR_BLOCK1) {	
			
		supla_esp_state.Relay[BL1_channel] = hi;
		supla_esp_save_state(SAVE_STATE_DELAY);
		supla_esp_channel_value_changed(BL1_channel, supla_esp_state.Relay[BL1_channel]);
		supla_esp_cfg_save(&supla_esp_cfg);
		supla_esp_channel_value_changed(BL1_channel, hi);
	}
	
if ( port == B_SENSOR_BLOCK2) {	
			
		supla_esp_state.Relay[BL2_channel] = hi;
		supla_esp_save_state(SAVE_STATE_DELAY);
		supla_esp_channel_value_changed(BL2_channel, supla_esp_state.Relay[BL2_channel]);
		supla_esp_cfg_save(&supla_esp_cfg);
		supla_esp_channel_value_changed(BL2_channel, hi);
	}	
}
