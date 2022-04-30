#include "color.h"

std::ostream &operator<<(std::ostream &os, Color const &c) {
  os << "Color(" << static_cast<int>(c.red) << ", " << static_cast<int>(c.green)
     << ", " << static_cast<int>(c.blue) << ")";
  return os;
}
