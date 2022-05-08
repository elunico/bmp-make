#include <cstdint>

#ifndef POINT_H
#define POINT_H

struct Point {
  uint32_t x, y;

  Point(uint32_t x, uint32_t y) : x(x), y(y) { }
};

#endif// POINT_H
