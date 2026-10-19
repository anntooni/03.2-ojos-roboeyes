
#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "config.h"

Adafruit_SSD1306 display(
    OLED_WIDTH,
    OLED_HEIGHT,
    &Wire,
    OLED_RESET_PIN
);

inline void initDisplay() {
    if (!display.begin(
            SSD1306_SWITCHCAPVCC,
            OLED_I2C_ADDRESS)) {
        Serial.println("[FATAL] Error al inicializar la pantalla OLED.");

        while (true) {
            delay(1000);
        }
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.display();

    Serial.printf(
        "[OLED] Pantalla inicializada: %dx%d, direccion 0x%02X\n",
        OLED_WIDTH,
        OLED_HEIGHT,
        OLED_I2C_ADDRESS
    );
}

inline void showText(
    const __FlashStringHelper* texto,
    int x,
    int y,
    uint8_t tamano
) {
    display.setTextSize(tamano);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(x, y);
    display.println(texto);
    display.display();
}

#endif
