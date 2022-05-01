#ifndef IMAGE_H
#define IMAGE_H

#include "color.h"
#include <cstdint>
#include <fstream>
#include <vector>

template <typename Integral> std::vector<uint8_t> get_le_bytes(Integral value) {
  std::vector<uint8_t> result{};
  result.reserve(sizeof(value));
  for (auto i = 0; i < sizeof(value); ++i) {
    result.push_back(static_cast<uint8_t>(value & 0xFF));
    value >>= 8;
  }
  return result;
}

struct Image {
  uint32_t width, height;
  std::vector<Color> pixels;

public:
  Image(uint32_t width, uint32_t height)
      : width(width), height(height), pixels(width * height) {}

  Color const &get_pixel(uint32_t x, uint32_t y) const;

  void set_pixel(uint32_t x, uint32_t y, Color const &c);

  uint32_t get_width() const;
  uint32_t get_height() const;
  uint32_t get_size() const;

  void write_to_file(std::string const &filename) const;

private:
  std::vector<uint8_t> file_header() const;

  std::vector<uint8_t> bmp_header() const;

  std::vector<uint8_t> image_data() const;

  std::vector<uint8_t> file_data() const;
};

#endif // IMAGE_H
