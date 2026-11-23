#ifndef DEBUG_SERIAL_H
#define DEBUG_SERIAL_H

#include <Arduino.h>
#include "config.h"
#include "motor_pwm.h"

inline void printMotorSpeed() {
    const uint16_t percentage =
        (uint16_t)motorSpeed * 100 / MOTOR_PWM_MAX_DUTY;
    Serial.printf("[MOTORES] velocidad %u%% (%u/255)\n",
                  (unsigned int)percentage,
                  (unsigned int)motorSpeed);
}

inline void printHelp() {
    Serial.println();
    Serial.println(F("===== CONTROL DEL ROBOT ====="));
    Serial.print(CONTROL_FORWARD);
    Serial.println(F(": adelante"));
    Serial.print(CONTROL_BACKWARD);
    Serial.println(F(": atras"));
    Serial.print(CONTROL_LEFT);
    Serial.println(F(": izquierda"));
    Serial.print(CONTROL_RIGHT);
    Serial.println(F(": derecha"));
    Serial.print(F("Espacio: detener"));
    Serial.println();
    Serial.print(CONTROL_SPEED_UP);
    Serial.println(F(": subir velocidad"));
    Serial.print(CONTROL_SPEED_DOWN);
    Serial.println(F(": bajar velocidad"));
}

inline void debugSerialTick() {
    if (Serial.available() <= 0) {
        return;
    }

    const char command = (char)Serial.read();
    if (command == '\n' || command == '\r' || command == '\t') {
        return;
    }

    switch (command) {
        case CONTROL_FORWARD:
        case 'w':
            moveForward();
            Serial.println(F("[MOTORES] adelante"));
            break;
        case CONTROL_BACKWARD:
        case 's':
            moveBackward();
            Serial.println(F("[MOTORES] atras"));
            break;
        case CONTROL_LEFT:
        case 'a':
            turnLeft();
            Serial.println(F("[MOTORES] izquierda"));
            break;
        case CONTROL_RIGHT:
        case 'd':
            turnRight();
            Serial.println(F("[MOTORES] derecha"));
            break;
        case CONTROL_STOP:
            stopMotors();
            Serial.println(F("[MOTORES] detenidos"));
            break;
        case CONTROL_SPEED_UP:
            setMotorSpeed((motorSpeed > MOTOR_PWM_MAX_DUTY - MOTOR_SPEED_STEP)
                              ? MOTOR_PWM_MAX_DUTY
                              : motorSpeed + MOTOR_SPEED_STEP);
            printMotorSpeed();
            break;
        case CONTROL_SPEED_DOWN:
            setMotorSpeed((motorSpeed < MOTOR_SPEED_STEP) ? 0 : motorSpeed - MOTOR_SPEED_STEP);
            printMotorSpeed();
            break;
        default:
            Serial.println(F("[SERIAL] comando no reconocido"));
            break;
    }
}

#endif
