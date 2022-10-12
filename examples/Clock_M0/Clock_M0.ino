/*

This sketch is designed for the Feather M0, and requires the ZeroRTC library.
Example of Oddly Specific Objects LCD FeatherWing
*/

#include <OSO_LCD.h>
#include <RTCZero.h> // https://github.com/arduino-libraries/RTCZero

RTCZero rtc;
OSO_LCDWing display;

void setup() {
    rtc.begin();
    // check that the LCD driver initialized successfully, and 
    // displays an error message on the serial terminal if it did not.
    if (!display.begin()) {
        while (1) Serial.println("ERR");
    }
    // clear the screen
    display.fill(0);
    // Set the Time
    rtc.setHours(11);
    rtc.setMinutes(59);
    rtc.setSeconds(55);
}

int minute = -1;
char buf[10];

void loop() {
    int newMinute = rtc.getMinutes();

    if (minute != newMinute) {
        int hours = rtc.getHours() % 12;
        minute = newMinute;
        sprintf(buf, "%2d%02d ", hours ? hours : 12, minute);
        display.clear_indicator(OSO_INDICATOR_ALL);
        if (rtc.getHours() < 12) {
            display.set_indicator(OSO_INDICATOR_AM);
        } else {
            display.set_indicator(OSO_INDICATOR_PM);
        }
        display.print(buf);
    }
    // Flash  the colon 
    display.toggle_colon();

    delay(500);
}
