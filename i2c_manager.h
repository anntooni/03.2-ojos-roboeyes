// i2c_manager.h
// ============================================
// RESPONSABILIDAD: Hablar con el bus I2C (pines, velocidad, escaneo y verificacion).
// No sabe nada de: OLED, logos, ojos ni comandos del Monitor Serie.
// ============================================

#ifndef I2C_MANAGER_H
#define I2C_MANAGER_H

#include <Arduino.h>
#include <Wire.h>
#include "config.h"

inline void initI2C() {
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    Wire.setClock(I2C_FREQUENCY_HZ);
    Serial.println(F("[I2C] bus listo SDA=21 SCL=22"));
}

inline void scanI2C() {
    byte count = 0;

    Serial.println(F("[I2C] escaneando direcciones 1-126"));
    for (byte address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        byte result = Wire.endTransmission();

        if (result == 0) {
            Serial.print(F("[I2C] dispositivo encontrado 0x"));
            if (address < 16) Serial.print('0');
            Serial.println(address, HEX);
            count++;
        }
    }

    Serial.print(F("[I2C] escaneo completo: "));
    Serial.print(count);
    Serial.println(count == 1 ? F(" dispositivo") : F(" dispositivos"));
}

inline void testI2CDevice() {
    Wire.beginTransmission(OLED_I2C_ADDRESS);
    byte result = Wire.endTransmission();

    if (result == 0) {
        Serial.print(F("[I2C] panel OLED responde en 0x"));
        Serial.println(OLED_I2C_ADDRESS, HEX);
    } else {
        Serial.print(F("[I2C] ERROR: panel OLED no responde en 0x"));
        Serial.println(OLED_I2C_ADDRESS, HEX);
        Serial.println(F("[BOOT] ERROR: el arranque se detiene"));
    }
}

#endif
