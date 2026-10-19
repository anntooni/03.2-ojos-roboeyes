#ifndef EYES_H
#define EYES_H

#include <Arduino.h>
#include "display.h"
#include "config.h"

#undef DEFAULT

#include <FluxGarage_RoboEyes.h>

RoboEyes<Adafruit_SSD1306> roboEyes(display);

inline void initEyes() {
    roboEyes.begin(OLED_WIDTH, OLED_HEIGHT, EYES_MAX_FPS);
    roboEyes.setMood(DEFAULT);
    Serial.println(F("[EYES] ojos listos a 60 fps"));
}

inline void updateEyes() {
    roboEyes.update();
}

inline void setEyesMood(char key) {
    roboEyes.setMood(DEFAULT);

    switch (key) {
        case '1':
            roboEyes.setMood(HAPPY);
            Serial.println(F("[EYES] expresion feliz"));
            break;

        case '2':
            roboEyes.setMood(ANGRY);
            Serial.println(F("[EYES] expresion enfadada"));
            break;

        case '3':
            roboEyes.setMood(TIRED);
            Serial.println(F("[EYES] expresion cansada"));
            break;

        case '4':
            roboEyes.setMood(SAD);
            Serial.println(F("[EYES] expresion triste"));
            break;

        case '5':
            roboEyes.setMood(DEFAULT);
            Serial.println(F("[EYES] expresion normal"));
            break;

        case '6':
            roboEyes.setMood(HAPPY);
            roboEyes.setPosition(E);
            Serial.println(F("[EYES] mirada a la derecha"));
            break;

        case '7':
            roboEyes.setMood(HAPPY);
            roboEyes.setPosition(W);
            Serial.println(F("[EYES] mirada a la izquierda"));
            break;

        default:
            Serial.println(F("[EYES] tecla no valida"));
            break;
    }
}

#endif