#pragma once

#include <Arduino.h>

// ============================================================================
// config.h - Configuracion global unica del proyecto (Semana 03.2)
// Sistema Embebido ESP32 - Motores, expresiones OLED y FSM de arranque
//
// Un solo archivo de configuracion por proyecto: aqui viven los parametros
// fisicos de hardware y los tiempos del sistema. Ningun modulo los redefine.
// ============================================================================

// --- Bus I2C (compartido por todos los perifericos del sistema) -------------
#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22
#define I2C_FREQUENCY_HZ 400000

// --- Pantalla OLED SSD1306 (unica pantalla del sistema) ---------------------
#define OLED_I2C_ADDRESS 0x3C
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_RESET_PIN -1

// --- Modulo de ojos (RoboEyes) ---------------------------------------------
#define EYES_MAX_FPS 60

// --- Puente H L293D: enable de cada motor y entradas de direccion -----------
#define MOTOR_LEFT_PWM_PIN 25
#define MOTOR_LEFT_IN1_PIN 27
#define MOTOR_LEFT_IN2_PIN 14
#define MOTOR_RIGHT_PWM_PIN 26
#define MOTOR_RIGHT_IN1_PIN 32
#define MOTOR_RIGHT_IN2_PIN 33
#define MOTOR_PWM_FREQUENCY_HZ 20000
#define MOTOR_PWM_RESOLUTION_BITS 8
#define MOTOR_PWM_MAX_DUTY 255
#define MOTOR_DEFAULT_SPEED 180
#define MOTOR_SPEED_STEP 20

// --- Maquina de estados de arranque (FSM BOOT -> RUN) ----------------------
#define LOGO_TIME_MS 3000
#define POST_DISPLAY_TIME_MS 350
