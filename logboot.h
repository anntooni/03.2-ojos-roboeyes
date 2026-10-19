
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
    Serial.println("[BOOT] Logo mostrado");
}

inline void testDisplay() {
    const int lado = 20;
    const int x = (OLED_WIDTH - lado) / 2;
    const int y = (OLED_HEIGHT - lado) / 2;

    display.clearDisplay();
    display.drawRect(x, y, lado, lado, SSD1306_WHITE);
    display.display();

    Serial.printf(
        "[POST] Cuadrado: x=%d, y=%d, ancho=%d, alto=%d\n",
        x,
        y,
        lado,
        lado
    );
}

#endif
