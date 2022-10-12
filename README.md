


[![OSO Logo ](https://cldup.com/NrFfLkzhjO.png)](https://www.oddlyspecificobjects.com/products/lcdwing/ "oddlyspecificobjects")

<a href="http://www.adafruit.com/products/5581"><img src="assets/5581-02.jpg?raw=true" width="500px"><br/>
Click here to purchase one from the Adafruit shop</a>


## Arduino Usage
In the Arduino library manager, search for and install the “Oddly Specific Objects LCD FeatherWing Library”. It may ask about a dependency, the “Adafruit Bus Device” library. If it does, make sure to install that library too.

Now, you can instantiate the LCD driver (after #includeing OSO_LCD.h):

	OSO_LCDWing display;
	if (!display.begin()) {
    while (1) Serial.println("ERR");
	}
	
This checks to make sure that the LCD driver initialized successfully, and displays an error message on the serial terminal if it did not.

You can clear the LCD like this:

	display.fill(0);
and energize all segments like so:

	display.fill(1);
Printing text to the LCD is easy:

	display.print("hello");
	

For more details, read onto the next section, notes for both platforms.

## Character Set Notes
The character set in the LCD FeatherWing library displays characters in mixed case in order to give each character a unique representation. Both uppercase and lowercase letters in your string display in the case chosen by the library; there is no difference between display.print("ABCDE") and display.print("abcde"). A appears as uppercase, but B and D appear as lowercase to distinguish them from the 8 and 0 characters:

![<Display Name>](https://www.oddlyspecificobjects.com/products/lcdwing/display-abcde.svg)

Similarly, C appears in lowercase to distinguish it from the open parenthesis (, as seen here when we display.print("(789)"):

![<Display Name>](https://www.oddlyspecificobjects.com/products/lcdwing/display-789.svg)

Some characters simply don’t work well as a seven-segment digit. In this example, we’ve called display.print("Wifi") and display.print("Main"). The W appears as an upside down A, and the M appears as a tall N:

![<Display Name>](https://www.oddlyspecificobjects.com/products/lcdwing/display-aifi.svg)
![<Display Name>](https://www.oddlyspecificobjects.com/products/lcdwing/display-nain.svg
)
If you are willing to sacrifice two characters, you can display these letters using some two character codes: $J displays a W, and &7 displays an M. So you could display “Wifi” by calling display.print("$Jifi"), or “Main” by calling display.print("&7ain"):

![<Display Name>](https://www.oddlyspecificobjects.com/products/lcdwing/display-wifi.svg
)

![<Display Name>](https://www.oddlyspecificobjects.com/products/lcdwing/display-main.svg
)

Finally, there’s one character that doesn’t do what you would expect. Since the # symbol doesn’t translate well to a 7-segment display, the library uses this character to display the degree symbol. So if you wanted to display a temperature of 72.4° F, you would do it like so: display.print("72.4#F").

![<Display Name>](https://www.oddlyspecificobjects.com/products/lcdwing/display-temperature.svg
)

## The Colon
When you call display.print, the LCD FeatherWing library automatically sets the colon if there is a colon in the appropriate spot (usually the third character in a string). So, for example, calling display.print("12:34") will set the colon.
![<Display Name>](https://www.oddlyspecificobjects.com/products/lcdwing/display-12_34.svg
)

However, calling display.print("1:23") will not set the colon.

![<Display Name>](https://www.oddlyspecificobjects.com/products/lcdwing/display-123.svg
)

Instead, add a space before: display.print(" 1:23"). The additional space at the beginning of the string pushes the colon to the correct spot.

![<Display Name>](https://www.oddlyspecificobjects.com/products/lcdwing/display-1_23.svg
)

There is also a special method to toggle the colon. Call display.toggle_colon() and it will set the colon to the opposite of its current state: if it was on, it turns off, and if it was off it turns on. Calling this method twice a second is a good way to display a clock ticking.

## Sign Indicator and Decimal Points
If the first character in your string is a minus sign (-), the LCD FeatherWing library will set the sign indicator to the left of the digits instead of putting the sign in the first digit. This means that you can display any number from -99999 to 99999.

![<Display Name>](https://www.oddlyspecificobjects.com/products/lcdwing/display-neg99999.svg
)
![<Display Name>](https://www.oddlyspecificobjects.com/products/lcdwing/display-99999.svg
)
It also means that if you are using the colon, it will be expected in the fourth position instead of the third, as in this countdown clock: display.print("-10:00.0").

![<Display Name>](https://www.oddlyspecificobjects.com/products/lcdwing/display-countdown.svg
)

There are four decimal points, one between each digit. The LCD FeatherWing library will automatically set the appropriate decimal point if it is included in your string, unless it occurs before the first digit or after the last one. This is because there is no decimal point to the left of the first digit or the right of the last one. So, for example, display.print(".1234") will display incorrectly, but display.print("0.1234") will work. Similarly, display.print("Thurs.") will omit the period at the end, but display.print("Thur.") will display it.

![<Display Name>](https://www.oddlyspecificobjects.com/products/lcdwing/display-01234.svg
)

![<Display Name>](https://www.oddlyspecificobjects.com/products/lcdwing/display-thur.svg
)

## Indicator Icons
There are seven indicators on the LCD, including five icons as well as the AM and PM indicators on the right.

![<Display Name>](https://www.oddlyspecificobjects.com/products/lcdwing/display-indicators.svg
)

You can control these indicators using the set_indicator and clear_indicator methods, but note that the names you pass to these functions differ between CircuitPython and Arduino.

## CircuitPython indicator names
The indicator names are defined in the Indicator class in the lcdwing_lite module. Their definitions, clockwise from top left:

*     Indicator.BELL
*     Indicator.WIFI
*     Indicator.AM
*     Indicator.PM
*     Indicator.BATTERY
*     Indicator.DATA (the paired arrows)
*     Indicator.MOON

There is also a special value, Indicator.ALL, that is useful to clear all indicators at once: display.clear_indicator(Indicator.ALL).

## Arduino indicator names
The indicator names are constants defined in OSO_LCD.h. Their names, clockwise from top left:

*     OSO_INDICATOR_BELL
*     OSO_INDICATOR_WIFI
*     OSO_INDICATOR_AM
*     OSO_INDICATOR_PM
*     OSO_INDICATOR_BATTERY
*     OSO_INDICATOR_DATA (the paired arrows)
*     OSO_INDICATOR_MOON

There is also a special value, OSO_INDICATOR_ALL, that is useful to clear all indicators at once: display.clear_indicator(OSO_INDICATOR_ALL).

