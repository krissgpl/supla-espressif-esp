/*
 Copyright (C) AC SOFTWARE SP. Z O.O.

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.
 */

#include "supla_hlw8012.h"

#include <osapi.h>

#define HLW8012_DEFAULT_STALE_TIME_US 2000000

static void ICACHE_FLASH_ATTR supla_hlw8012_clear_measurements(
    supla_hlw8012_t *hlw) {
  hlw->last_cf_ts = 0;
  hlw->last_cf1_ts = 0;
  hlw->cf_period_us = 0;
  hlw->cf1_period_us = 0;
  hlw->cached_voltage_v = 0;
  hlw->cached_current_a = 0;
}

void ICACHE_FLASH_ATTR supla_hlw8012_init(supla_hlw8012_t *hlw,
                                          const supla_hlw8012_config_t *cfg) {
  if (hlw == NULL || cfg == NULL) {
    return;
  }

  memset(hlw, 0, sizeof(supla_hlw8012_t));
  memcpy(&hlw->cfg, cfg, sizeof(supla_hlw8012_config_t));

  if (hlw->cfg.stale_time_us == 0) {
    hlw->cfg.stale_time_us = HLW8012_DEFAULT_STALE_TIME_US;
  }
}

void ICACHE_FLASH_ATTR supla_hlw8012_set_cf1_mode(supla_hlw8012_t *hlw,
                                                  uint8 cf1_mode) {
  if (hlw == NULL) {
    return;
  }

  if (cf1_mode != SUPLA_HLW8012_CF1_MODE_VOLTAGE &&
      cf1_mode != SUPLA_HLW8012_CF1_MODE_CURRENT) {
    return;
  }

  hlw->cf1_mode = cf1_mode;
}

void ICACHE_FLASH_ATTR supla_hlw8012_on_cf_pulse(supla_hlw8012_t *hlw,
                                                 uint32 timestamp_us) {
  uint32 period_us = 0;

  if (hlw == NULL) {
    return;
  }

  if (hlw->last_cf_ts != 0) {
    period_us = timestamp_us - hlw->last_cf_ts;
    if (period_us != 0) {
      hlw->cf_period_us = period_us;
      hlw->energy_wh += hlw->cfg.power_multiplier / 3600.0f;
    }
  }

  hlw->last_cf_ts = timestamp_us;
}

void ICACHE_FLASH_ATTR supla_hlw8012_on_cf1_pulse(supla_hlw8012_t *hlw,
                                                  uint32 timestamp_us) {
  uint32 period_us = 0;
  float freq_hz = 0;

  if (hlw == NULL) {
    return;
  }

  if (hlw->last_cf1_ts != 0) {
    period_us = timestamp_us - hlw->last_cf1_ts;
    if (period_us != 0) {
      hlw->cf1_period_us = period_us;
      freq_hz = 1000000.0f / period_us;

      if (hlw->cf1_mode == SUPLA_HLW8012_CF1_MODE_VOLTAGE) {
        hlw->cached_voltage_v = freq_hz * hlw->cfg.voltage_multiplier;
      } else {
        hlw->cached_current_a = freq_hz * hlw->cfg.current_multiplier;
      }
    }
  }

  hlw->last_cf1_ts = timestamp_us;
}

void ICACHE_FLASH_ATTR supla_hlw8012_reset_energy(supla_hlw8012_t *hlw) {
  if (hlw == NULL) {
    return;
  }

  hlw->energy_wh = 0;
}

void ICACHE_FLASH_ATTR supla_hlw8012_get_values(supla_hlw8012_t *hlw,
                                                uint32 timestamp_us,
                                                supla_hlw8012_values_t *values) {
  uint32 age_cf = 0;
  uint32 age_cf1 = 0;

  if (hlw == NULL || values == NULL) {
    return;
  }

  memset(values, 0, sizeof(supla_hlw8012_values_t));

  if (hlw->last_cf_ts != 0) {
    age_cf = timestamp_us - hlw->last_cf_ts;
  }

  if (hlw->last_cf1_ts != 0) {
    age_cf1 = timestamp_us - hlw->last_cf1_ts;
  }

  if (hlw->last_cf_ts == 0 || age_cf > hlw->cfg.stale_time_us) {
    supla_hlw8012_clear_measurements(hlw);
    values->energy_wh = hlw->energy_wh;
    return;
  }

  if (hlw->cf_period_us != 0) {
    values->power_w = 1000000.0f / hlw->cf_period_us * hlw->cfg.power_multiplier;
  }

  if (hlw->last_cf1_ts != 0 && age_cf1 <= hlw->cfg.stale_time_us) {
    values->voltage_v = hlw->cached_voltage_v;
    values->current_a = hlw->cached_current_a;
  }

  values->energy_wh = hlw->energy_wh;

  if (values->voltage_v > 0.01f && values->current_a > 0.0001f) {
    values->power_factor =
        values->power_w / (values->voltage_v * values->current_a);
    if (values->power_factor < 0) {
      values->power_factor = 0;
    } else if (values->power_factor > 1) {
      values->power_factor = 1;
    }
  }
}
