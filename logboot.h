// logboot.h
// ============================================
// RESPONSABILIDAD: Dibujar el logo de arranque y hacer el POST de pantalla.
// No sabe nada de: ojos, bus I2C ni comandos del Monitor Serie.
// ============================================

#ifndef LOGBOOT_H
#define LOGBOOT_H

#include <Arduino.h>
#include "display.h"
#include "logo.h"

inline void showLogo() {
    display.clearDisplay();
    display.drawBitmap(0, 0, logo_bitmap, LOGO_WIDTH, LOGO_HEIGHT, SSD1306_WHITE);
    display.display();
}

inline void testDisplay() {
    const int size = 16;
    const int x = (OLED_WIDTH - size) / 2;
    const int y = (OLED_HEIGHT - size) / 2;

    display.clearDisplay();
    display.drawRect(x, y, size, size, SSD1306_WHITE);
    display.display();

    Serial.print(F("[POST] cuadrado centrado en x="));
    Serial.print(x);
    Serial.print(F(" y="));
    Serial.println(y);
}

#endif
