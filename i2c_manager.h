
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
    uint8_t encontrados = 0;

    Serial.println("[I2C] Escaneo de direcciones 0x01 a 0x7E");

    for (uint8_t direccion = 0x01; direccion <= 0x7E; direccion++) {
        Wire.beginTransmission(direccion);
        uint8_t error = Wire.endTransmission();

        if (error == 0) {
            Serial.printf(
                "[I2C] Dispositivo encontrado en 0x%02X\n",
                direccion
            );
            encontrados++;
        }
    }

    Serial.printf(
        "[I2C] Escaneo finalizado. Dispositivos encontrados: %u\n",
        encontrados
    );
}

inline void testI2CDevice() {
    Wire.beginTransmission(OLED_I2C_ADDRESS);
    uint8_t error = Wire.endTransmission();

    if (error == 0) {
        Serial.printf(
            "[I2C] OLED detectada en 0x%02X: OK\n",
            OLED_I2C_ADDRESS
        );
    } else {
        Serial.printf(
            "[FATAL] OLED sin respuesta en 0x%02X. Error I2C: %u\n",
            OLED_I2C_ADDRESS,
            error
        );

        while (true) {
            delay(1000);
        }
    }
}

#endif
