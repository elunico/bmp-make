#ifndef COLOR_H
#define COLOR_H

#include <cstdint>
#include <iostream>

static int isLittleEndian() {
  volatile uint32_t i = 0x01234567;
  // return 0 for big endian, 1 for little endian.
  return (*((uint8_t*) (&i))) == 0x67;
}

struct Color {
  uint8_t red, green, blue;

  Color(uint8_t red, uint8_t green, uint8_t blue) : red(red), green(green), blue(blue) { }

  Color() = default;

  friend std::ostream& operator<<(std::ostream& os, Color const& c);

  operator int() const noexcept;

  static Color const Empty;
};


#endif// COLOR_H
