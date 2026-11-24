// ESP32-8048S070C (7" 800x480, GT911 touch) - simple LVGL demo (label saja)
// Env PlatformIO: simple_demo
#include <Arduino.h>
#include <esp32_smartdisplay.h>

// Pengukuran CPU% untuk LVGL sysmon (LV_OS_NONE)
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

    // WAJIB: init LCD + touch + LVGL + backlight
    smartdisplay_init();
    smartdisplay_lcd_set_backlight(1.0f);

    lv_obj_t *label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "Halo dari ESP32-S3!");
    lv_obj_set_style_text_font(label, &lv_font_montserrat_24, 0);
    lv_obj_center(label);
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
