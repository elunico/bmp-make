#ifndef COLOR_H
#define COLOR_H

#include <cstdint>
#include <iostream>

struct Color {
  uint8_t red, green, blue;

public:
  Color(uint8_t red, uint8_t green, uint8_t blue)
      : red(red), green(green), blue(blue) {}
  Color() = default;

  friend std::ostream &operator<<(std::ostream &os, Color const &c);
};

#endif // COLOR_H
