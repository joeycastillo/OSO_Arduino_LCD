/*!
 * @file OSO_LCD.h
 */

#ifndef OSO_LCD_h
#define OSO_LCD_h

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Adafruit_I2CDevice.h>

#define OSO_INDICATOR_AM (0b10000000)
#define OSO_INDICATOR_PM (0b01000000)
#define OSO_INDICATOR_BATTERY (0b00100000)
#define OSO_INDICATOR_BELL (0b00001000)
#define OSO_INDICATOR_WIFI (0b00000100)
#define OSO_INDICATOR_DATA (0b00000010)
#define OSO_INDICATOR_MOON (0b00000001)
#define OSO_INDICATOR_ALL (0b11101111)

class OSO_BU9796 {
public:
  OSO_BU9796(void);
  bool begin(uint8_t _addr = 0x3e, TwoWire *theWire = &Wire);
  void show(void);
  void show_partial(uint8_t length, uint8_t pos = 0);
  void fill(bool on);

  bool auto_write = true;

protected:
  void _write_cmd(uint8_t cmd);
  void _set_buffer(uint8_t pos, uint8_t value);
  uint8_t _get_buffer(uint8_t pos);
  uint8_t _buffer[7];
  Adafruit_I2CDevice *i2c_dev = NULL;
};

class OSO_LCDWing : public OSO_BU9796 {
public:
  OSO_LCDWing(void) {};
  void print(char *s);
  void set_indicator(uint8_t indicator);
  void clear_indicator(uint8_t indicator);
  void toggle_colon();
protected:
  void _update_byte_0();
};

#endif // OSO_LCD_h
