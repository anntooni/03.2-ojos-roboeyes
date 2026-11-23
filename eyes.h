// eyes.h
// ============================================
// RESPONSABILIDAD: Animar los ojos del OLED y aplicar la expresion elegida.
// No sabe nada de: bus I2C, logos de arranque, POST ni Monitor Serie.
// ============================================

#ifndef EYES_H
#define EYES_H

#include <Arduino.h>
#include "display.h"
#include "config.h"

// Arduino.h del ESP32 define DEFAULT como 1 y RoboEyes lo define como 0. Se
// limpia esa macro (sin uso en el core) para evitar el aviso de redefinicion.
#undef DEFAULT

#include <FluxGarage_RoboEyes.h>

// Instancia global: el sistema tiene un solo par de ojos
RoboEyes<Adafruit_SSD1306> roboEyes(display);

inline void initEyes() {
    roboEyes.begin(OLED_WIDTH, OLED_HEIGHT, EYES_MAX_FPS);
    Serial.println(F("[OJOS] ojos listos a 60 fps"));
}

inline void updateEyes() {
    roboEyes.update();
}

inline void setEyesMood(char key) {
    roboEyes.setMood(DEFAULT);
    roboEyes.setAutoblinker(false);
    roboEyes.setIdleMode(false);
    roboEyes.setCuriosity(false);
    roboEyes.setSweat(false);

    switch (key) {
        case '1': roboEyes.setMood(DEFAULT); break;
        case '2': roboEyes.setMood(HAPPY); break;
        case '3': roboEyes.setMood(ANGRY); break;
        case '4': roboEyes.setMood(TIRED); break;
        case '5': roboEyes.setMood(TIRED); roboEyes.blink(); break;
        case '6': roboEyes.setMood(ANGRY); roboEyes.setSweat(true); break;
        case '7': roboEyes.setMood(DEFAULT); roboEyes.setCuriosity(true); break;
    }
}

#endif
