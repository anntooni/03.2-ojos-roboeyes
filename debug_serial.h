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
#include "motor_manager.h"

inline void printHelp() {
    Serial.println(F("[DEBUG] ayuda:"));
    Serial.println(F("1 DEFAULT | 2 HAPPY | 3 ANGRY"));
    Serial.println(F("4 TIRED | 5 SLEEPY | 6 SCARY"));
    Serial.println(F("7 CURIOUS | h ayuda"));
    Serial.println(F("Motores: W adelante | X atras | S detener"));
    Serial.println(F("A/Z motor A | J/N motor B"));
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
        } else if (key == 'W' || key == 'w') {
            setMotorA(MOTOR_FORWARD);
            setMotorB(MOTOR_FORWARD);
            Serial.println(F("[MOTOR] ambos adelante"));
        } else if (key == 'X' || key == 'x') {
            setMotorA(MOTOR_BACKWARD);
            setMotorB(MOTOR_BACKWARD);
            Serial.println(F("[MOTOR] ambos atras"));
        } else if (key == 'A' || key == 'a') {
            setMotorA(MOTOR_FORWARD);
            Serial.println(F("[MOTOR] A adelante"));
        } else if (key == 'Z' || key == 'z') {
            setMotorA(MOTOR_BACKWARD);
            Serial.println(F("[MOTOR] A atras"));
        } else if (key == 'J' || key == 'j') {
            setMotorB(MOTOR_FORWARD);
            Serial.println(F("[MOTOR] B adelante"));
        } else if (key == 'N' || key == 'n') {
            setMotorB(MOTOR_BACKWARD);
            Serial.println(F("[MOTOR] B atras"));
        } else if (key == 'S' || key == 's') {
            stopMotors();
            Serial.println(F("[MOTOR] ambos detenidos"));
        }
    }
}

#endif
