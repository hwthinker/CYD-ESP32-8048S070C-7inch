// ESP32-8048S070C (7" 800x480, GT911 touch) - LVGL 9 Widgets demo (profil "Elena Smith")
#include <Arduino.h>
#include <esp32_smartdisplay.h>
#include <demos/lv_demos.h>

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
    lv_timer_handler();
}
