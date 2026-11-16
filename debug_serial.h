#ifndef DEBUG_SERIAL_H
#define DEBUG_SERIAL_H

#include <Arduino.h>
#include <ctype.h>
#include "config.h"
#include "eyes.h"
#include "motor_pwm.h"

inline void reportMotionCommand(MotionState requestedMotion, const __FlashStringHelper* label) {
    if (currentMotion == requestedMotion) {
        Serial.print(F("[MOTORES] "));
        Serial.println(label);
    }
}

inline void printHelp() {
    Serial.println();
    Serial.println(F("===== CONTROL DEL ROBOT ====="));
    Serial.println(F("W - adelante | S - atras | A - girar izquierda | D - girar derecha"));
    Serial.println(F("Espacio o 0 - detener | + / - - subir o bajar velocidad"));
    Serial.println(F("1 - feliz | 2 - enfadado | 3 - cansado | 4 - triste"));
    Serial.println(F("5 - normal | 6 - mirada derecha | 7 - mirada izquierda"));
    Serial.println(F("Deten los motores antes de cambiar a los ojos; H - ayuda"));
}

inline void debugSerialTick() {
    if (Serial.available() <= 0) {
        return;
    }

    const char key = (char)Serial.read();
    if (key == '\n' || key == '\r' || key == '\t' || key < 32 || key > 126) {
        return;
    }

    const char command = (char)tolower((unsigned char)key);
    switch (command) {
        case 'w':
            moveForward();
            reportMotionCommand(MOTION_FORWARD, F("adelante"));
            break;
        case 's':
            moveBackward();
            reportMotionCommand(MOTION_BACKWARD, F("atras"));
            break;
        case 'a':
            turnLeft();
            reportMotionCommand(MOTION_LEFT, F("giro a la izquierda"));
            break;
        case 'd':
            turnRight();
            reportMotionCommand(MOTION_RIGHT, F("giro a la derecha"));
            break;
        case ' ':
        case '0':
            stopMotors();
            Serial.println(F("[MOTORES] detenidos"));
            break;
        case '+':
        case '=':
            setMotorSpeed((motorSpeed > MOTOR_PWM_MAX_DUTY - MOTOR_SPEED_STEP)
                              ? MOTOR_PWM_MAX_DUTY
                              : motorSpeed + MOTOR_SPEED_STEP);
            Serial.printf("[MOTORES] velocidad %u/255\n", (unsigned int)motorSpeed);
            break;
        case '-':
            setMotorSpeed((motorSpeed < MOTOR_SPEED_STEP) ? 0 : motorSpeed - MOTOR_SPEED_STEP);
            Serial.printf("[MOTORES] velocidad %u/255\n", (unsigned int)motorSpeed);
            break;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
            setEyesMood(command);
            break;
        case 'h':
            printHelp();
            break;
        default:
            Serial.println(F("[SERIAL] comando no reconocido"));
            break;
    }
}

#endif
