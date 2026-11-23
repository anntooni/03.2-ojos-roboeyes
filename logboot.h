#ifndef LOGBOOT_H
#define LOGBOOT_H

#include <Arduino.h>
#include "display.h"
#include "logo.h"

inline void showLogo() {
    display.clearDisplay();
    display.drawBitmap(
        0,
        0,
        logo_bitmap,
        LOGO_WIDTH,
        LOGO_HEIGHT,
        SSD1306_WHITE
    );
    display.display();
    Serial.println(F("[BOOT] Logo mostrado"));
}

inline void testDisplay() {
    const int16_t squareSize = 20;
    const int16_t x = (OLED_WIDTH - squareSize) / 2;
    const int16_t y = (OLED_HEIGHT - squareSize) / 2;

    display.clearDisplay();
    display.drawRect(x, y, squareSize, squareSize, SSD1306_WHITE);
    display.display();
    delay(POST_DISPLAY_TIME_MS);

    Serial.printf(
        "[POST] Cuadrado: x=%d, y=%d, ancho=%d, alto=%d\n",
        x,
        y,
        squareSize,
        squareSize
    );
}

#endif
