#include "color.h"

std::ostream& operator<<(std::ostream& os, Color const& c) {
  os << "Color(" << static_cast<int>(c.red) << ", " << static_cast<int>(c.green) << ", " << static_cast<int>(c.blue)
     << ")";
  return os;
}

Color::operator int() const noexcept {
  if (isLittleEndian()) {
    return (0xff << 24) | (blue << 16) | (green << 8) | red;
  } else {
    return (red << 24) | (green << 16) | (blue << 8) | 0xff;
  }
}

Color const Color::Empty{ 0, 0, 0 };
