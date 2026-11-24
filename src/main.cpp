// ESP32-8048S070C (7" 800x480, GT911 touch) - LVGL 9 Widgets demo (profil "Elena Smith")
#include <Arduino.h>
#include <esp32_smartdisplay.h>
#include <demos/lv_demos.h>

// ---------------------------------------------------------------------------
// Pengukuran CPU% untuk LVGL sysmon.
// LVGL memakai LV_OS_NONE, jadi idle harus diukur sendiri: CPU% = waktu yang
// dipakai lv_timer_handler() dibagi total waktu loop.
// ---------------------------------------------------------------------------
static uint32_t s_busy_us = 0;
static uint32_t s_total_us = 0;

extern "C" uint32_t esp_sysmon_get_idle(void)
{
    uint32_t idle = 100;
    if (s_total_us > 0)
    {
        uint32_t cpu = (uint32_t)((uint64_t)s_busy_us * 100u / s_total_us);
        idle = (cpu >= 100u) ? 0u : (100u - cpu);
    }
    s_busy_us = 0;
    s_total_us = 0;
    return idle;
}

void setup()
{
    Serial.begin(115200);
    log_i("Board: %s", BOARD_NAME);
    log_i("Free heap: %d, PSRAM: %d", ESP.getFreeHeap(), ESP.getPsramSize());

    smartdisplay_init();
    smartdisplay_lcd_set_backlight(1.0f); // kecerahan penuh

    lv_demo_widgets();
}

auto lv_last_tick = millis();

void loop()
{
    auto const now = millis();
    lv_tick_inc(now - lv_last_tick);
    lv_last_tick = now;

    static uint32_t last_us = micros();
    uint32_t t0 = micros();
    lv_timer_handler();
    uint32_t t1 = micros();

    s_busy_us += (t1 - t0);
    s_total_us += (t1 - last_us);
    last_us = t1;
}
