#ifndef DEBUG_SERIAL_H
#define DEBUG_SERIAL_H

#include <Arduino.h>
#include "config.h"
#include "eyes.h"

inline void printHelp() {
    Serial.println();
    Serial.println(F("===== CONTROL DE OJOS ROBOEYES ====="));
    Serial.println(F("1 - Feliz"));
    Serial.println(F("2 - Enfadado"));
    Serial.println(F("3 - Cansado"));
    Serial.println(F("4 - Triste"));
    Serial.println(F("5 - Normal"));
    Serial.println(F("6 - Mirada a la derecha"));
    Serial.println(F("7 - Mirada a la izquierda"));
    Serial.println(F("h - Mostrar esta ayuda"));
    Serial.println(F("==================================="));
}

inline void debugSerialTick() {
    if (Serial.available() <= 0) {
        return;
    }

    char key = Serial.read();

    if (key == '\n' || key == '\r' || key == '\t' || key < 32 || key > 126) {
        return;
    }

    if (key == 'h' || key == 'H') {
        printHelp();
        return;
    }

    if (key >= '1' && key <= '7') {
        setEyesMood(key);
    } else {
        Serial.println(F("[SERIAL] comando no reconocido"));
    }
}

#endif