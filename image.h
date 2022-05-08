#ifndef IMAGE_H
#define IMAGE_H

#include "color.h"
#include "point.h"
#include <cstdint>
#include <fstream>
#include <vector>

struct Close { };

std::fstream& operator<<(std::fstream& os, Close);

constexpr static const Close CLOSE{};

template <typename Integral>
std::vector<uint8_t> get_le_bytes(Integral value) {
  std::vector<uint8_t> result{};
  result.reserve(sizeof(value));
  for (auto i = 0; i < sizeof(value); ++i) {
    result.push_back(static_cast<uint8_t>(value & 0xFF));
    value >>= 8;
  }
  return result;
}

template <typename Integral>
std::vector<uint8_t> get_be_bytes(Integral value) {
  std::vector<uint8_t> result{};
  result.reserve(sizeof(value));
  auto mask = static_cast<Integral>(0xFF) << (sizeof(value) * 8 - 8);
  for (auto i = sizeof(value); i > 0; --i) {
    result.push_back(static_cast<uint8_t>((value & mask) >> (i - 1) * 8));
    value >>= 8;
  }
  return result;
}

struct Image {
  uint32_t width, height;
  std::vector<Color> pixels;

public:
  Image(uint32_t width, uint32_t height) : width(width), height(height), pixels(width * height) { }

  [[nodiscard]] Color const& get_pixel(uint32_t x, uint32_t y) const;

  void set_pixel(uint32_t x, uint32_t y, Color const& c);

  Color const& operator[](Point const& p) const;

  Color& operator[](Point const& p);

  [[nodiscard]] uint32_t get_width() const;

  [[nodiscard]] uint32_t get_height() const;

  [[nodiscard]] uint32_t get_size() const;

  void write_to_file(std::string const& filename) const;

  friend std::fstream& operator<<(std::fstream& os, Image const& image);

  [[nodiscard]] std::vector<uint8_t> file_header() const;

  [[nodiscard]] std::vector<uint8_t> bmp_header() const;

  [[nodiscard]] std::vector<uint8_t> image_data() const;

  [[nodiscard]] std::vector<uint8_t> file_data() const;
};

#endif// IMAGE_H
