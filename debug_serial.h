// debug_serial.h
// ============================================
// RESPONSABILIDAD: Leer comandos del Monitor Serie y mostrar la ayuda.
// No sabe nada de: bus I2C, OLED, logos ni animacion interna de los ojos.
// ============================================

#ifndef DEBUG_SERIAL_H
#define DEBUG_SERIAL_H

#include <Arduino.h>
#include "config.h"
#include "eyes.h"

inline void printHelp() {
    Serial.println(F("[DEBUG] ayuda:"));
    Serial.println(F("1 DEFAULT | 2 HAPPY | 3 ANGRY"));
    Serial.println(F("4 TIRED | 5 SLEEPY | 6 SCARY"));
    Serial.println(F("7 CURIOUS | h ayuda"));
}

inline void debugSerialTick() {
    if (Serial.available()) {
        char key = Serial.read();

        if (key == 'h') {
        printHelp();
        } else if (key >= '1' && key <= '7') {
        setEyesMood(key);
        Serial.print(F("[DEBUG] expresion "));
        Serial.println(key);
        }
    }
}

#endif
