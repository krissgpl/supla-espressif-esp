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

#include <stdio.h>
#include <string.h>

#include <ip_addr.h>
#include <mem.h>
#include <osapi.h>
#include <user_interface.h>

#include "supla-dev/log.h"
#include "supla_dht.h"
#include "supla_ds18b20.h"
#include "supla_esp.h"
#include "supla_esp_cfg.h"
#include "supla_esp_cfgmode.h"
#include "supla_esp_countdown_timer.h"
#include "supla_esp_devconn.h"
#include "supla_esp_dns_client.h"
#include "supla_esp_electricity_meter.h"
#include "supla_esp_gpio.h"
#include "supla_esp_impulse_counter.h"
#include "supla_esp_pwm.h"

#include "board/supla_esp_board.c"

#if ((SPI_FLASH_SIZE_MAP == 0) || (SPI_FLASH_SIZE_MAP == 1))
#error "The flash map is not supported"
#elif (SPI_FLASH_SIZE_MAP == 2)
#error "The flash map is not supported"
#elif (SPI_FLASH_SIZE_MAP == 3)
#error "The flash map is not supported"
#elif (SPI_FLASH_SIZE_MAP == 4)
#error "The flash map is not supported"
#elif (SPI_FLASH_SIZE_MAP == 5)
#define SYSTEM_PARTITION_OTA_SIZE 0xE0000
#define SYSTEM_PARTITION_OTA_2_ADDR 0x101000
#define SYSTEM_PARTITION_RF_CAL_ADDR 0x1fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR 0x1fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR 0x1fd000
#define SYSTEM_PARTITION_AT_PARAMETER_ADDR 0xfd000
#define SYSTEM_PARTITION_SSL_CLIENT_CERT_PRIVKEY_ADDR 0xfc000
#define SYSTEM_PARTITION_SSL_CLIENT_CA_ADDR 0xfb000
#define SYSTEM_PARTITION_WPA2_ENTERPRISE_CERT_PRIVKEY_ADDR 0xfa000
#define SYSTEM_PARTITION_WPA2_ENTERPRISE_CA_ADDR 0xf9000
#elif (SPI_FLASH_SIZE_MAP == 6)
#define SYSTEM_PARTITION_OTA_SIZE 0xE0000
#define SYSTEM_PARTITION_OTA_2_ADDR 0x101000
#define SYSTEM_PARTITION_RF_CAL_ADDR 0x3fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR 0x3fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR 0x3fd000
#define SYSTEM_PARTITION_AT_PARAMETER_ADDR 0xfd000
#define SYSTEM_PARTITION_SSL_CLIENT_CERT_PRIVKEY_ADDR 0xfc000
#define SYSTEM_PARTITION_SSL_CLIENT_CA_ADDR 0xfb000
#define SYSTEM_PARTITION_WPA2_ENTERPRISE_CERT_PRIVKEY_ADDR 0xfa000
#define SYSTEM_PARTITION_WPA2_ENTERPRISE_CA_ADDR 0xf9000
#else
#error "The flash map is not supported"
#endif

static const partition_item_t at_partition_table[] = {
    {SYSTEM_PARTITION_BOOTLOADER, 0x0, 0x1000},
    {SYSTEM_PARTITION_OTA_1, 0x1000, SYSTEM_PARTITION_OTA_SIZE},
    {SYSTEM_PARTITION_OTA_2, SYSTEM_PARTITION_OTA_2_ADDR,
     SYSTEM_PARTITION_OTA_SIZE},
    {SYSTEM_PARTITION_RF_CAL, SYSTEM_PARTITION_RF_CAL_ADDR, 0x1000},
    {SYSTEM_PARTITION_PHY_DATA, SYSTEM_PARTITION_PHY_DATA_ADDR, 0x1000},
    {SYSTEM_PARTITION_SYSTEM_PARAMETER, SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR,
     0x3000},
    {SYSTEM_PARTITION_AT_PARAMETER, SYSTEM_PARTITION_AT_PARAMETER_ADDR, 0x3000},
    {SYSTEM_PARTITION_SSL_CLIENT_CERT_PRIVKEY,
     SYSTEM_PARTITION_SSL_CLIENT_CERT_PRIVKEY_ADDR, 0x1000},
    {SYSTEM_PARTITION_SSL_CLIENT_CA, SYSTEM_PARTITION_SSL_CLIENT_CA_ADDR,
     0x1000},
#ifdef CONFIG_AT_WPA2_ENTERPRISE_COMMAND_ENABLE
    {SYSTEM_PARTITION_WPA2_ENTERPRISE_CERT_PRIVKEY,
     SYSTEM_PARTITION_WPA2_ENTERPRISE_CERT_PRIVKEY_ADDR, 0x1000},
    {SYSTEM_PARTITION_WPA2_ENTERPRISE_CA,
     SYSTEM_PARTITION_WPA2_ENTERPRISE_CA_ADDR, 0x1000},
#endif
};

#ifdef __FOTA
#include "supla_update.h"
#endif

typedef struct {
  uint32 cycles;
  uint32 last_system_time;
  ETSTimer timer;
} _t_usermain_uptime;

_t_usermain_uptime usermain_uptime;

unsigned _supla_int64_t MAIN_ICACHE_FLASH uptime_usec(void) {
  uint32 time = system_get_time();
  if (time < usermain_uptime.last_system_time) {
    usermain_uptime.cycles++;
  }
  usermain_uptime.last_system_time = time;
  return usermain_uptime.cycles * (unsigned _supla_int64_t)0xffffffff +
         (unsigned _supla_int64_t)time;
}

unsigned _supla_int64_t MAIN_ICACHE_FLASH uptime_msec(void) {
  return uptime_usec() / (unsigned _supla_int64_t)1000;
}

uint32 MAIN_ICACHE_FLASH uptime_sec(void) {
  return uptime_msec() / (unsigned _supla_int64_t)1000;
}

void MAIN_ICACHE_FLASH supla_system_restart(void) {
#ifdef BOARD_IS_RESTART_ALLOWED
  if (supla_esp_board_is_restart_allowed() == 0) {
    return;
  }
#endif

  supla_esp_devconn_before_system_restart();

#ifdef BOARD_BEFORE_REBOOT
  supla_esp_board_before_reboot();
#endif

  supla_log(LOG_DEBUG, "RESTART");
  supla_log(LOG_DEBUG, "Free heap size: %i", system_get_free_heap_size());

  system_restart();
}

uint32 MAIN_ICACHE_FLASH user_rf_cal_sector_set(void) {
  enum flash_size_map size_map = system_get_flash_size_map();
  uint32 rf_cal_sec = 0;

  switch (size_map) {
    case FLASH_SIZE_4M_MAP_256_256:
      rf_cal_sec = 128 - 5;
      break;

    case FLASH_SIZE_8M_MAP_512_512:
      rf_cal_sec = 256 - 5;
      break;

    case FLASH_SIZE_16M_MAP_512_512:
    case FLASH_SIZE_16M_MAP_1024_1024:
      rf_cal_sec = 512 - 5;
      break;

    case FLASH_SIZE_32M_MAP_512_512:
    case FLASH_SIZE_32M_MAP_1024_1024:
      rf_cal_sec = 1024 - 5;
      break;

    default:
      rf_cal_sec = 0;
      break;
  }

  return rf_cal_sec;
}

void MAIN_ICACHE_FLASH user_rf_pre_init(){};

void MAIN_ICACHE_FLASH supla_esp_uptime_counter_watchdog_cb(void *ptr) {
  uptime_usec();
}

void MAIN_ICACHE_FLASH user_pre_init(void) {
  if (!system_partition_table_regist(
          at_partition_table,
          sizeof(at_partition_table) / sizeof(at_partition_table[0]),
          SPI_FLASH_SIZE_MAP)) {
    os_printf("system_partition_table_regist fail\r\n");
  }
}

void MAIN_ICACHE_FLASH user_init(void) {
  memset(&usermain_uptime, 0, sizeof(_t_usermain_uptime));

  os_timer_disarm(&usermain_uptime.timer);
  os_timer_setfn(&usermain_uptime.timer,
                 (os_timer_func_t *)supla_esp_uptime_counter_watchdog_cb, NULL);
  os_timer_arm(&usermain_uptime.timer, 10000, 1);

#ifdef BOARD_USER_INIT
  BOARD_USER_INIT;
#else

  struct rst_info *rtc_info = system_get_rst_info();
  supla_log(LOG_DEBUG, "RST reason: %i", rtc_info->reason);

  system_soft_wdt_restart();

  wifi_status_led_uninstall();
  supla_esp_cfg_init();
  supla_esp_gpio_init();

  supla_log(LOG_DEBUG, "Starting %i", system_get_time());

#ifdef BOARD_ESP_STARTING
  BOARD_ESP_STARTING;
#endif

#if NOSSL == 1
  supla_log(LOG_DEBUG, "NO SSL!");
#endif

#ifdef __FOTA
  supla_esp_update_init();
#endif

#ifndef COUNTDOWN_TIMER_DISABLED
  supla_esp_countdown_timer_init();
#endif /*COUNTDOWN_TIMER_DISABLED*/

#ifndef ADDITIONAL_DNS_CLIENT_DISABLED
  supla_esp_dns_client_init();
#endif /*ADDITIONAL_DNS_CLIENT_DISABLED*/

  supla_esp_devconn_init();
	 
	#if defined TEMP_SELECT
	
		if ( supla_esp_cfg.ThermometerType == 1 ) {
			supla_ds18b20_init();
			supla_log(LOG_DEBUG, "DS18B20 INIT" );
		}
		
		if ( supla_esp_cfg.ThermometerType == 2 ) {
			supla_dht_init();
			supla_log(LOG_DEBUG, "DHT22 INIT" );
		}
	#else
		
#ifdef DS18B20
  supla_ds18b20_init();
		 supla_log(LOG_DEBUG, "DS18B20 old INIT" );
#endif

#ifdef DHTSENSOR
  supla_dht_init();
  supla_log(LOG_DEBUG, "DHT22 old INIT" );
#endif

#ifdef SUPLA_PWM_COUNT
  supla_esp_pwm_init();
#endif

#ifdef ELECTRICITY_METER_COUNT
  supla_esp_em_init();
#endif

#ifdef IMPULSE_COUNTER_COUNT
  supla_esp_ic_init();
#endif

  if (((supla_esp_cfg.LocationID == 0 || supla_esp_cfg.LocationPwd[0] == 0) &&
       supla_esp_cfg.Email[0] == 0) ||
      supla_esp_cfg.Server[0] == 0 || supla_esp_cfg.WIFI_PWD[0] == 0 ||
      supla_esp_cfg.WIFI_SSID[0] == 0) {
    supla_esp_cfgmode_start();
    return;
  }

	#if defined TEMP_SELECT
	
		if ( supla_esp_cfg.ThermometerType == 1 ) {
			supla_ds18b20_start();
			supla_log(LOG_DEBUG, "DS18B20 START" );
		}
		
		if ( supla_esp_cfg.ThermometerType == 2 ) {
			supla_dht_start();
			supla_log(LOG_DEBUG, "DHT22 START" );
		}
	#else

     #ifdef DS18B20
  supla_ds18b20_start();
  supla_log(LOG_DEBUG, "DS18B20 old START" );
     #endif

	 #ifdef DHTSENSOR
  supla_dht_start();
  supla_log(LOG_DEBUG, "DHT22 old START" );
	 #endif
#endif

#ifdef ELECTRICITY_METER_COUNT
  supla_esp_em_start();
#endif

#ifdef IMPULSE_COUNTER_COUNT
  supla_esp_ic_start();
#endif

  supla_esp_devconn_start();

  system_print_meminfo();

#ifdef BOARD_ESP_STARTED
  BOARD_ESP_STARTED;
#endif

#endif /*BOARD_USER_INIT*/
}
