// motor_display.h
// ============================================
// RESPONSABILIDAD: Mostrar el estado del movimiento en el OLED sin bloquear.
// ============================================

#ifndef MOTOR_DISPLAY_H
#define MOTOR_DISPLAY_H

#include <Arduino.h>
#include "config.h"
#include "display.h"
#include "motor_pwm.h"

static const unsigned long MOTOR_DISPLAY_FRAME_MS = 120;
unsigned long lastMotorDisplayFrame = 0;
uint8_t motorDisplayPhase = 0;

inline const __FlashStringHelper* motionLabel() {
    switch (currentMotion) {
        case MOTION_FORWARD: return F("ADELANTE");
        case MOTION_BACKWARD: return F("ATRAS");
        case MOTION_LEFT: return F("IZQUIERDA");
        case MOTION_RIGHT: return F("DERECHA");
        case MOTION_STOPPED:
        default: return F("DETENIDO");
    }
}

inline void drawMotionArrow() {
    const int16_t offset = motorDisplayPhase ? 3 : 0;
    const int16_t x = 64;
    const int16_t y = 27 + offset;

    switch (currentMotion) {
        case MOTION_FORWARD:
            display.drawFastVLine(x, y - 10, 25, SSD1306_WHITE);
            display.drawLine(x, y - 10, x - 8, y - 2, SSD1306_WHITE);
            display.drawLine(x, y - 10, x + 8, y - 2, SSD1306_WHITE);
            break;
        case MOTION_BACKWARD:
            display.drawFastVLine(x, y - 10, 25, SSD1306_WHITE);
            display.drawLine(x, y + 15, x - 8, y + 7, SSD1306_WHITE);
            display.drawLine(x, y + 15, x + 8, y + 7, SSD1306_WHITE);
            break;
        case MOTION_LEFT:
            display.drawFastHLine(48 - offset, y, 33, SSD1306_WHITE);
            display.drawLine(48 - offset, y, 56 - offset, y - 8, SSD1306_WHITE);
            display.drawLine(48 - offset, y, 56 - offset, y + 8, SSD1306_WHITE);
            break;
        case MOTION_RIGHT:
            display.drawFastHLine(48 + offset, y, 33, SSD1306_WHITE);
            display.drawLine(80 + offset, y, 72 + offset, y - 8, SSD1306_WHITE);
            display.drawLine(80 + offset, y, 72 + offset, y + 8, SSD1306_WHITE);
            break;
        case MOTION_STOPPED:
        default:
            display.drawRoundRect(52, 23, 24, 24, 4, SSD1306_WHITE);
            display.fillRect(60, 30, 8, 10, SSD1306_WHITE);
            break;
    }
}

inline void drawMovementStatus() {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print(F("ROBOT: "));
    display.println(motionLabel());
    drawMotionArrow();
    display.drawFastHLine(0, 53, OLED_WIDTH, SSD1306_WHITE);
    display.setCursor(0, 56);
    display.print(F("Velocidad: "));
    display.print((uint16_t)motorSpeed * 100 / MOTOR_PWM_MAX_DUTY);
    display.print(F("%"));
    display.display();
}

inline void updateMovementDisplay() {
    if (!movementDisplayActive) {
        return;
    }

    const unsigned long now = millis();
    const bool animating = currentMotion != MOTION_STOPPED;
    if (!movementDisplayDirty &&
        (!animating || now - lastMotorDisplayFrame < MOTOR_DISPLAY_FRAME_MS)) {
        return;
    }

    if (animating) {
        motorDisplayPhase ^= 1;
    }
    drawMovementStatus();
    lastMotorDisplayFrame = now;
    movementDisplayDirty = false;
}

#endif
