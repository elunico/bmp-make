#include "image.h"

Color const &Image::get_pixel(uint32_t x, uint32_t y) const {
  try {
    return pixels.at(y * width + x);
  } catch (...) {
    std::cout << "Out of bounds: " << x << ", " << y << std::endl;
    throw std::out_of_range("Out of range");
  }
}

void Image::set_pixel(uint32_t x, uint32_t y, Color const &c) {
  pixels[y * width + x] = c;
}

uint32_t Image::get_width() const { return width; }
uint32_t Image::get_height() const { return height; }
uint32_t Image::get_size() const { return width * height; }

void Image::write_to_file(std::string const &filename) const {
  auto data = file_data();
  std::ofstream ofs(filename, std::ios::binary);
  ofs.write(reinterpret_cast<char const *>(data.data()), data.size());
}

std::vector<uint8_t> Image::file_header() const {
  auto result = std::vector<uint8_t>();
  result.reserve(14);
  result.push_back(static_cast<uint8_t>('B'));
  result.push_back(static_cast<uint8_t>('M'));
  auto p = get_le_bytes(54 + get_size() * 3);
  result.insert(result.end(), p.begin(), p.end());
  for (int i = 0; i < 4; i++) {
    result.push_back(static_cast<uint8_t>(0));
  }
  p = get_le_bytes(54);
  result.insert(result.end(), p.begin(), p.end());
  return result;
}

std::vector<uint8_t> Image::bmp_header() const {
  auto result = std::vector<uint8_t>();
  result.reserve(40);
  auto p = get_le_bytes(40);
  result.insert(result.end(), p.begin(), p.end());
  p = get_le_bytes(width);
  result.insert(result.end(), p.begin(), p.end());
  p = get_le_bytes(height);
  result.insert(result.end(), p.begin(), p.end());
  p = get_le_bytes(static_cast<uint16_t>(1));
  result.insert(result.end(), p.begin(), p.end());
  p = get_le_bytes(static_cast<uint16_t>(24));
  result.insert(result.end(), p.begin(), p.end());
  for (int i = 0; i < 24; i++) {
    result.push_back(static_cast<uint8_t>(0));
  }
  return result;
}

std::vector<uint8_t> Image::image_data() const {
  auto result = std::vector<uint8_t>();
  result.reserve(get_size() * 3);
  auto padding = (4 - ((width * 3) % 4)) % 4;

  // TODO: MISSING LAST LINE
  for (auto y = height - 1;; --y) {
    if (y >= height)
      break;
    for (auto x = 0; x < width; ++x) {
      auto const &c = get_pixel(x, y);
      result.push_back(c.blue);
      result.push_back(c.green);
      result.push_back(c.red);
    }
    for (auto i = 0; i < padding; ++i) {
      result.push_back(static_cast<uint8_t>(0));
    }
  }
  return result;
}

std::vector<uint8_t> Image::file_data() const {
  auto result = std::vector<uint8_t>();
  auto fheader = file_header();
  auto bheader = bmp_header();
  auto data = image_data();
  result.reserve(fheader.size() + bheader.size() + data.size());
  result.insert(result.end(), fheader.begin(), fheader.end());
  result.insert(result.end(), bheader.begin(), bheader.end());
  result.insert(result.end(), data.begin(), data.end());
  return result;
}
