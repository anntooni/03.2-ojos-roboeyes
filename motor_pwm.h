// motor_pwm.h
// ============================================
// RESPONSABILIDAD: Controlar velocidad y direccion de los motores con PWM.
// Las salidas se conectan a las entradas del puente H L293D.
// ============================================

#ifndef MOTOR_PWM_H
#define MOTOR_PWM_H

#include <Arduino.h>
#include "config.h"
#include <esp_arduino_version.h>

enum MotionState : uint8_t {
    MOTION_STOPPED,
    MOTION_FORWARD,
    MOTION_BACKWARD,
    MOTION_LEFT,
    MOTION_RIGHT
};

static const uint8_t MOTOR_LEFT_PWM_CHANNEL = 0;
static const uint8_t MOTOR_RIGHT_PWM_CHANNEL = 1;

MotionState currentMotion = MOTION_STOPPED;
uint8_t motorSpeed = MOTOR_DEFAULT_SPEED;
bool motorPwmReady = false;
bool movementDisplayActive = true;
bool movementDisplayDirty = true;

inline bool attachMotorPwm(uint8_t pin, uint8_t channel) {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
    (void)channel;
    return ledcAttach(pin, MOTOR_PWM_FREQUENCY_HZ, MOTOR_PWM_RESOLUTION_BITS);
#else
    ledcSetup(channel, MOTOR_PWM_FREQUENCY_HZ, MOTOR_PWM_RESOLUTION_BITS);
    ledcAttachPin(pin, channel);
    return true;
#endif
}

inline bool writeMotorPwm(uint8_t pin, uint8_t channel, uint8_t duty) {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
    (void)channel;
    return ledcWrite(pin, duty);
#else
    (void)pin;
    ledcWrite(channel, duty);
    return true;
#endif
}

inline void writeMotorOutputs(bool leftForward, bool rightForward) {
    if (!motorPwmReady) {
        return;
    }

    const uint8_t leftDuty = (currentMotion == MOTION_STOPPED) ? 0 : motorSpeed;
    const uint8_t rightDuty = (currentMotion == MOTION_STOPPED) ? 0 : motorSpeed;
    const bool leftDisabled =
        writeMotorPwm(MOTOR_LEFT_PWM_PIN, MOTOR_LEFT_PWM_CHANNEL, 0);
    const bool rightDisabled =
        writeMotorPwm(MOTOR_RIGHT_PWM_PIN, MOTOR_RIGHT_PWM_CHANNEL, 0);

    if (!leftDisabled || !rightDisabled) {
        currentMotion = MOTION_STOPPED;
        digitalWrite(MOTOR_LEFT_IN1_PIN, LOW);
        digitalWrite(MOTOR_LEFT_IN2_PIN, LOW);
        digitalWrite(MOTOR_RIGHT_IN1_PIN, LOW);
        digitalWrite(MOTOR_RIGHT_IN2_PIN, LOW);
        Serial.println(F("[MOTORES] ERROR: no se pudo desactivar el PWM"));
        return;
    }

    digitalWrite(MOTOR_LEFT_IN1_PIN, leftForward ? HIGH : LOW);
    digitalWrite(MOTOR_LEFT_IN2_PIN, leftForward ? LOW : HIGH);
    digitalWrite(MOTOR_RIGHT_IN1_PIN, rightForward ? HIGH : LOW);
    digitalWrite(MOTOR_RIGHT_IN2_PIN, rightForward ? LOW : HIGH);

    const bool leftApplied =
        writeMotorPwm(MOTOR_LEFT_PWM_PIN, MOTOR_LEFT_PWM_CHANNEL, leftDuty);
    const bool rightApplied =
        writeMotorPwm(MOTOR_RIGHT_PWM_PIN, MOTOR_RIGHT_PWM_CHANNEL, rightDuty);
    if (!leftApplied || !rightApplied) {
        writeMotorPwm(MOTOR_LEFT_PWM_PIN, MOTOR_LEFT_PWM_CHANNEL, 0);
        writeMotorPwm(MOTOR_RIGHT_PWM_PIN, MOTOR_RIGHT_PWM_CHANNEL, 0);
        digitalWrite(MOTOR_LEFT_IN1_PIN, LOW);
        digitalWrite(MOTOR_LEFT_IN2_PIN, LOW);
        digitalWrite(MOTOR_RIGHT_IN1_PIN, LOW);
        digitalWrite(MOTOR_RIGHT_IN2_PIN, LOW);
        currentMotion = MOTION_STOPPED;
        Serial.println(F("[MOTORES] ERROR: fallo al escribir el PWM"));
    }
}

inline void initMotorPwm() {
    pinMode(MOTOR_LEFT_IN1_PIN, OUTPUT);
    pinMode(MOTOR_LEFT_IN2_PIN, OUTPUT);
    pinMode(MOTOR_RIGHT_IN1_PIN, OUTPUT);
    pinMode(MOTOR_RIGHT_IN2_PIN, OUTPUT);
    digitalWrite(MOTOR_LEFT_IN1_PIN, LOW);
    digitalWrite(MOTOR_LEFT_IN2_PIN, LOW);
    digitalWrite(MOTOR_RIGHT_IN1_PIN, LOW);
    digitalWrite(MOTOR_RIGHT_IN2_PIN, LOW);

    motorPwmReady = attachMotorPwm(MOTOR_LEFT_PWM_PIN, MOTOR_LEFT_PWM_CHANNEL) &&
                    attachMotorPwm(MOTOR_RIGHT_PWM_PIN, MOTOR_RIGHT_PWM_CHANNEL);
    if (!motorPwmReady) {
        Serial.println(F("[MOTORES] ERROR: no se pudo enlazar el PWM; motores deshabilitados"));
        return;
    }

    Serial.printf("[MOTORES] PWM listo: %u Hz, %u bits\n",
                  (unsigned int)MOTOR_PWM_FREQUENCY_HZ,
                  (unsigned int)MOTOR_PWM_RESOLUTION_BITS);
}

inline void applyMotion() {
    if ((!motorPwmReady || motorSpeed == 0) && currentMotion != MOTION_STOPPED) {
        const bool pwmUnavailable = !motorPwmReady;
        currentMotion = MOTION_STOPPED;
        if (pwmUnavailable) {
            Serial.println(F("[MOTORES] ERROR: PWM no disponible; movimiento cancelado"));
        } else {
            Serial.println(F("[MOTORES] velocidad en cero; movimiento cancelado"));
        }
    }

    switch (currentMotion) {
        case MOTION_FORWARD:
            writeMotorOutputs(true, true);
            break;
        case MOTION_BACKWARD:
            writeMotorOutputs(false, false);
            break;
        case MOTION_LEFT:  // Para girar en el sitio, las ruedas van en sentidos opuestos.
            writeMotorOutputs(false, true);
            break;
        case MOTION_RIGHT:
            writeMotorOutputs(true, false);
            break;
        case MOTION_STOPPED:
        default:
            if (motorPwmReady) {
                const bool leftStopped =
                    writeMotorPwm(MOTOR_LEFT_PWM_PIN, MOTOR_LEFT_PWM_CHANNEL, 0);
                const bool rightStopped =
                    writeMotorPwm(MOTOR_RIGHT_PWM_PIN, MOTOR_RIGHT_PWM_CHANNEL, 0);
                digitalWrite(MOTOR_LEFT_IN1_PIN, LOW);
                digitalWrite(MOTOR_LEFT_IN2_PIN, LOW);
                digitalWrite(MOTOR_RIGHT_IN1_PIN, LOW);
                digitalWrite(MOTOR_RIGHT_IN2_PIN, LOW);
                if (!leftStopped || !rightStopped) {
                    Serial.println(F("[MOTORES] ERROR: fallo al detener el PWM"));
                }
            }
            break;
    }
    movementDisplayActive = true;
    movementDisplayDirty = true;
}

inline void moveForward() {
    currentMotion = MOTION_FORWARD;
    applyMotion();
}

inline void moveBackward() {
    currentMotion = MOTION_BACKWARD;
    applyMotion();
}

inline void turnLeft() {
    currentMotion = MOTION_LEFT;
    applyMotion();
}

inline void turnRight() {
    currentMotion = MOTION_RIGHT;
    applyMotion();
}

inline void stopMotors() {
    currentMotion = MOTION_STOPPED;
    applyMotion();
}

inline void setMotorSpeed(uint8_t speed) {
    motorSpeed = speed;
    movementDisplayActive = true;
    if (motorSpeed == 0) {
        currentMotion = MOTION_STOPPED;
    }
    applyMotion();
}

#endif
