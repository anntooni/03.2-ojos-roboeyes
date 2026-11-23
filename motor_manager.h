// motor_manager.h
// ============================================
// RESPONSABILIDAD: Controlar ambos motores y reflejar su direccion real
// en las entradas IN que alimentan los cuatro LED indicadores.
// ============================================

#pragma once

#include <Arduino.h>
#include "config.h"

enum MotorDirection {
    MOTOR_STOP,
    MOTOR_FORWARD,
    MOTOR_BACKWARD,
    MOTOR_BRAKE
};

void setMotorDirection(
    MotorDirection direction,
    int forwardPin,
    int backwardPin,
    int pwmChannel
) {
    bool forward = direction == MOTOR_FORWARD;
    bool backward = direction == MOTOR_BACKWARD;

    // LOW/LOW mantiene los dos indicadores apagados y detiene el motor.
    // Para la simulacion no se usa frenado HIGH/HIGH porque ambos LED
    // quedarían encendidos al estar conectados directamente a las entradas.
    if (direction == MOTOR_BRAKE) forward = false, backward = false;

    digitalWrite(forwardPin, forward ? HIGH : LOW);
    digitalWrite(backwardPin, backward ? HIGH : LOW);

    if (direction == MOTOR_STOP || direction == MOTOR_BRAKE) {
        ledcWrite(pwmChannel, 0);
    } else {
        ledcWrite(pwmChannel, MOTOR_DEFAULT_SPEED);
    }
}

inline void initMotors() {
    pinMode(MOTOR_A_ENABLE_PIN, OUTPUT);
    pinMode(MOTOR_A_FORWARD_PIN, OUTPUT);
    pinMode(MOTOR_A_BACKWARD_PIN, OUTPUT);
    pinMode(MOTOR_B_ENABLE_PIN, OUTPUT);
    pinMode(MOTOR_B_FORWARD_PIN, OUTPUT);
    pinMode(MOTOR_B_BACKWARD_PIN, OUTPUT);

    ledcSetup(MOTOR_A_PWM_CHANNEL, MOTOR_PWM_FREQUENCY_HZ, MOTOR_PWM_RESOLUTION);
    ledcSetup(MOTOR_B_PWM_CHANNEL, MOTOR_PWM_FREQUENCY_HZ, MOTOR_PWM_RESOLUTION);
    ledcAttachPin(MOTOR_A_ENABLE_PIN, MOTOR_A_PWM_CHANNEL);
    ledcAttachPin(MOTOR_B_ENABLE_PIN, MOTOR_B_PWM_CHANNEL);

    setMotorDirection(
        MOTOR_STOP,
        MOTOR_A_FORWARD_PIN,
        MOTOR_A_BACKWARD_PIN,
        MOTOR_A_PWM_CHANNEL
    );
    setMotorDirection(
        MOTOR_STOP,
        MOTOR_B_FORWARD_PIN,
        MOTOR_B_BACKWARD_PIN,
        MOTOR_B_PWM_CHANNEL
    );

    ledcWrite(MOTOR_A_PWM_CHANNEL, 0);
    ledcWrite(MOTOR_B_PWM_CHANNEL, 0);
}

inline void setMotorA(MotorDirection direction) {
    setMotorDirection(
        direction,
        MOTOR_A_FORWARD_PIN,
        MOTOR_A_BACKWARD_PIN,
        MOTOR_A_PWM_CHANNEL
    );
}

inline void setMotorB(MotorDirection direction) {
    setMotorDirection(
        direction,
        MOTOR_B_FORWARD_PIN,
        MOTOR_B_BACKWARD_PIN,
        MOTOR_B_PWM_CHANNEL
    );
}

inline void stopMotors() {
    setMotorA(MOTOR_STOP);
    setMotorB(MOTOR_STOP);
}