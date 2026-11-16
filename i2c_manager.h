#ifndef I2C_MANAGER_H
#define I2C_MANAGER_H

#include <Arduino.h>
#include <Wire.h>
#include "config.h"

inline void initI2C() {
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    Wire.setClock(I2C_FREQUENCY_HZ);

    Serial.printf(
        "[I2C] Bus iniciado: SDA=%d, SCL=%d, frecuencia=%lu Hz\n",
        I2C_SDA_PIN,
        I2C_SCL_PIN,
        (unsigned long)I2C_FREQUENCY_HZ
    );
}

inline void scanI2C() {
    uint8_t devicesFound = 0;
    Serial.println(F("[I2C] Escaneo de direcciones 0x01 a 0x7E"));

    for (uint8_t address = 0x01; address <= 0x7E; ++address) {
        Wire.beginTransmission(address);
        if (Wire.endTransmission() == 0) {
            Serial.printf(
                "[I2C] Dispositivo encontrado en 0x%02X\n",
                (unsigned int)address
            );
            ++devicesFound;
        }
    }

    Serial.printf(
        "[I2C] Escaneo finalizado. Dispositivos encontrados: %u\n",
        (unsigned int)devicesFound
    );
}

inline void testI2CDevice() {
    Wire.beginTransmission(OLED_I2C_ADDRESS);
    const uint8_t error = Wire.endTransmission();

    if (error != 0) {
        Serial.printf(
            "[FATAL] OLED sin respuesta en 0x%02X. Error I2C: %u\n",
            (unsigned int)OLED_I2C_ADDRESS,
            (unsigned int)error
        );
        while (true) {
            delay(1000);
        }
    }

    Serial.printf(
        "[I2C] OLED detectada en 0x%02X: OK\n",
        (unsigned int)OLED_I2C_ADDRESS
    );
}

#endif
