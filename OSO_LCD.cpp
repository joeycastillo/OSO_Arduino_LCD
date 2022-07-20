/*!
 * @file OSO_LCD.cpp
 */

#include "OSO_LCD.h"

static const PROGMEM uint8_t CHARS[] = {
    0b00000000, // [space]
    0b00101100, // !
    0b01001000, // "
    0b11001010, // # (°)
    0b01010101, // $ (Like an L with an extra segment, use $J to make a W)
    0b01000100, // %
    0b11011000, // & (Like an F without a cross stroke, use &7 to make an M)
    0b00001000, // '
    0b11010001, // (
    0b10001101, // )
    0b00000000, // * (Currently unused)
    0b01010010, // +
    0b00000100, // ,
    0b00000010, // -
    0b00100000, // .
    0b00001100, // /
    0b11011101, // 0
    0b00001100, // 1
    0b10011011, // 2
    0b10001111, // 3
    0b01001110, // 4
    0b11000111, // 5
    0b11010111, // 6
    0b11001100, // 7
    0b11011111, // 8
    0b11001111, // 9
    0b00000000, // : (RESERVED: special character that turns on the colon)
    0b01000000, // ; (Like the apostrophe, but on the other side, use 'foo; to say ‘foo’)
    0b00010001, // <
    0b00000011, // =
    0b00000101, // >
    0b10011010, // ?
    0b11111111, // @
    0b11011110, // A
    0b01010111, // B
    0b00010011, // C
    0b00011111, // D
    0b11010011, // E
    0b11010010, // F
    0b11001111, // G
    0b01010110, // H
    0b00010000, // I
    0b00011101, // J
    0b11010110, // K
    0b01010001, // L
    0b11011100, // M
    0b00010110, // N
    0b00010111, // O
    0b11011010, // P
    0b11001110, // Q
    0b00010010, // R
    0b11000101, // S
    0b01010011, // T
    0b00010101, // U
    0b01011101, // V
    0b01011111, // W
    0b01011110, // X
    0b01001111, // Y
    0b10011001, // Z
};

OSO_BU9796::OSO_BU9796(void) {
  memset(this->_buffer, 0, 7);
}

bool OSO_BU9796::begin(uint8_t _addr, TwoWire *theWire) {
  if (this->i2c_dev) delete i2c_dev;
  this->i2c_dev = new Adafruit_I2CDevice(_addr, theWire);
  if (!this->i2c_dev->begin()) return false;

  this->_write_cmd(0b00111100); // Configure for lowest power consumption
  this->_write_cmd(0b01001000); // display ON, 1/3 bias

  return true;
}

void OSO_BU9796::show(void) {
  this->i2c_dev->write(this->_buffer, 7);
}

void OSO_BU9796::show_partial(uint8_t length, uint8_t pos) {
  if (pos) {
    uint8_t temp[6];
    temp[0] = pos * 2;
    memcpy(temp + 1, this->_buffer + pos + 1, length);
    this->i2c_dev->write(temp, length + 1);
  } else {
    this->i2c_dev->write(this->_buffer, length + 1);
  }
}

void OSO_BU9796::fill(bool on) {
  for (int i = 0; i < 6; i++) {
    this->_buffer[i + 1] = on ? 0xFF : 0x00;
  }
  if (this->auto_write) this->show();
}

void OSO_BU9796::_write_cmd(uint8_t cmd) {
  this->i2c_dev->write(&cmd, 1);
}

void OSO_BU9796::_set_buffer(uint8_t pos, uint8_t value) {
  this->_buffer[pos + 1] = value;
}

uint8_t OSO_BU9796::_get_buffer(uint8_t pos) {
  return this->_buffer[pos + 1];
}

void OSO_LCDWing::print(char *s) {
  char value[10];
  for(int i = 0; i < strlen(s); i++) value[i] = toupper(s[i]);
  bool neg = value[0] == '-';
  int pos = neg ? 1 : 0;
  bool d = false;
  int i = 1;
  this->_set_buffer(0, this->_get_buffer(0) & ~0b00010000);
  while(i < 6) {
    int b = 0;
    char c = value[pos];
    pos++;
    if (c == '.') {
      d = true;
      continue;
    } else if (c == ':') {
      if (i == 3) this->_set_buffer(0, this->_get_buffer(0) | 0b00010000);
      continue;
    }
    b = CHARS[c - 32];
    this->_set_buffer(i, b | (((neg and i == 1) || d ) ? 0b00100000 : 0));
    i += 1;
    d = false;
  }
  if (this->auto_write) {
    this->show();
  }
}

void OSO_LCDWing::set_indicator(uint8_t indicator) {
  this->_set_buffer(0, this->_get_buffer(0) | indicator);
  this->_update_byte_0();
}

void OSO_LCDWing::clear_indicator(uint8_t indicator) {
  this->_set_buffer(0, this->_get_buffer(0) & ~indicator);
  this->_update_byte_0();
}

void OSO_LCDWing::toggle_colon() {
  uint8_t indicators = this->_get_buffer(0);
  this->_set_buffer(0, (indicators & 0b00010000) ? (indicators & ~0b00010000) : (indicators | 0b00010000));
  this->_update_byte_0();
}

void OSO_LCDWing::_update_byte_0() {
  if (this->auto_write) this->show_partial(1);
}
