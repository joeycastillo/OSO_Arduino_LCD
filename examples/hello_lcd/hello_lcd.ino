#include <OSO_LCD.h>

OSO_LCDWing display;

void setup() {
    if (!display.begin()) {
        while (1) Serial.println("ERR");
    }
    display.fill(0);

    display.print("HELLO");
}

void loop() {
}
