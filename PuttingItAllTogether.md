[![OSO Kogo ](https://cldup.com/NrFfLkzhjO.png)](https://www.oddlyspecificobjects.com/products/lcdwing/ "oddlyspecificobjects")


## Putting It All Together: a Simple Clock App

![<Display Name>](https://www.oddlyspecificobjects.com/products/lcdwing/display-clock.svg)

## CircuitPython
This Python script is available in the examples folder of the LCD FeatherWing CircuitPython library. It’s a simple clock that uses CircuitPython’s built-in RTC module to display the current time.

>     import board
>     import time
>     import rtc
>     from oso_lcd.lcdwing_lite import LCDWingLite, Indicator
>     
>     display = LCDWingLite(board.I2C())
>     minute = None
>     clock = rtc.RTC()
>     clock.datetime = time.struct_time((2022, 6, 30, 11, 59, 55, 0, -1, -1))
>     
>     while True:
>         if minute != clock.datetime.tm_min:
>             dt = clock.datetime
>             hour = dt.tm_hour % 12
>             minute = dt.tm_min
>             display.clear_indicator(Indicator.ALL)
>             display.print("{:2d}:{:02d}".format(hour if hour else 12, minute))
>             if dt.tm_hour < 12:
>                 display.set_indicator(Indicator.AM)
>             else:
>                 display.set_indicator(Indicator.PM)
>         display.toggle_colon()
>         time.sleep(0.5)
>     

## Arduino
This Arduino sketch is available in the examples folder of the LCD FeatherWing Arduino library. It functions identically to the CircuitPython script above. Note that it is designed for the Feather M0, and requires the ZeroRTC library.



    #include <OSO_LCD.h>
    #include <RTCZero.h>
    
    RTCZero rtc;
    OSO_LCDWing display;
    
    void setup() {
        rtc.begin();
        if (!display.begin()) {
            while (1) Serial.println("ERR");
        }
        display.fill(0);
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
        display.toggle_colon();
    
        delay(500);
    }
    
## Resources

* [LCD FeatherWing CircuitPython library](https://github.com/joeycastillo/OSO_CircuitPython_LCD)

* [LCD FeatherWing Arduino library](https://github.com/joeycastillo/OSO_Arduino_LCD)
* [Design files for LCD wing and custom LCD](https://github.com/joeycastillo/LCD-FeatherWing)
* [BU9796 data sheet](https://www.oddlyspecificobjects.com/products/lcdwing/bu9796axxx-e.pdf)


## Advanced Configuration
There are two solder jumpers on the back of the circuit board that allow for advanced configuration. In general, you don’t need to do anything to them, but they can be useful in some specific cases.

JP1: Software Contrast Control
JP1 allows you to control the LCD driving voltage using the DAC pin on an ESP32 Feather or a Feather M4. This can be useful to lower the contrast of the display if you intend to view it from an extreme angle.

To enable this functionality, use an Exacto knife to cut the bridge connecting JP1 to the pad on the right (GND). Then use a blob of solder to bridge the center pad to the left pad (A0).

Now, the voltage on A0 will define the LCD voltage according to the following formula: VLCD = 3.3 - A0. Setting A0 to 0 V will drive the LCD at 3.3 volts; setting A0 to 0.3 V will drive it at 3.0 volts, etc.

SDA and SCL jumpers: alternate I²C bus
The BU9796 has no address pins, so you can only drive one display on an I²C bus. However, some Feather boards can support two or even more I²C buses! By soldering wires to these pads and rerouting some signals, you can drive multiple LCD FeahterWings from one Feather.

Please note: THIS IS AN ADVANCED HACK and it’s easy to inadvertently cut a trace, pull a pad or knock off a surface mount resistor while doing this. Please only attempt this if you feel comfortable working in a very small area.

To do this hack, fully cut the bridge connecting the top and bottom of the SDA and SCL jumpers. Then solder a wire to the BOTTOM half of the jumper. Connect the other side of the wire to an alternate SDA or SCL pin.

Some guidance on which pins work:

ESP32/S2/S3 and NRF52840 Feathers
These devices have two I²C peripherals. In addition to the main I²C bus on SDA and SCL, any two pins can function as a second I²C bus.

Feather M0 and M0 Express
These devices can support up to four I²C buses. In addition to the main I²C bus on SDA and SCL, you can use the following pins to create three additional I²C buses:

* A1 (SDA) and A2 (SCL) *
* A3 (SDA) and A4 (SCL)
* 11 (SDA) and 13 (SCL)

Note that in this configuration, you will be unable to use the SPI pins as an SPI bus, although they’ll still work fine as GPIO.

## Feather M4 Express
This device can support up to SIX I²C buses! In addition to the main I²C bus on SDA and SCL, you can use the following pins to create five additional I²C buses:

* A2 (SDA) and A3 (SCL)
* A4 (SDA) and A1 (SCL)
* 12 (SDA) and 13 (SCL)
* TX (SDA) and RX (SCL)
* 5 (SDA) and SCK (SCL)




