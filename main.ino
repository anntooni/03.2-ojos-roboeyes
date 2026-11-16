#include "config.h"
#include "i2c_manager.h"
#include "display.h"
#include "logo.h"
#include "logboot.h"
#include "motor_pwm.h"
#include "motor_display.h"
#include "eyes.h"
#include "debug_serial.h"

bool bootComplete = false;
unsigned long bootTime = 0;

void setup() {
    Serial.begin(115200);
    Serial.println(F("[BOOT] RoboEyes OLED y control de motores"));

    initI2C();
    scanI2C();
    testI2CDevice();
    initDisplay();
    testDisplay();
    initEyes();
    showLogo();
    initMotorPwm();
    stopMotors();
    printHelp();

    bootTime = millis();
    Serial.println(F("[BOOT] ventana de arranque iniciada"));
}

void loop() {
    if (!bootComplete && millis() - bootTime >= LOGO_TIME_MS) {
        bootComplete = true;
        movementDisplayDirty = true;
        Serial.println(F("[BOOT] ventana finalizada; estado RUN"));
    }

    if (!bootComplete) {
        return;
    }

    debugSerialTick();
    if (movementDisplayActive) {
        updateMovementDisplay();
    } else {
        updateEyes();
    }
}
