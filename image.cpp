#include "image.h"
#include "precondition.h"

template <typename T>
void vec_concat_inplace(std::vector<T>& destination, std::vector<T> const& source) {
  destination.insert(destination.end(), source.begin(), source.end());
}

Color const& Image::get_pixel(uint32_t x, uint32_t y) const {
  try {
    return pixels.at(y * width + x);
  } catch (...) {
    std::cerr << "Out of bounds: " << x << ", " << y << std::endl;
    throw std::out_of_range("Out of range");
  }
}

void Image::set_pixel(uint32_t x, uint32_t y, Color const& c) {
  precondition(x < width, "x is out of bounds");
  precondition(y < height, "y is out of bounds");
  pixels[y * width + x] = c;
}

Color const& Image::operator[](Point const& p) const { return pixels[p.y * width + p.x]; }

Color& Image::operator[](Point const& p) { return pixels[p.y * width + p.x]; }

uint32_t Image::get_width() const { return width; }

uint32_t Image::get_height() const { return height; }

uint32_t Image::get_size() const { return width * height; }

void Image::write_to_file(std::string const& filename) const {
  auto data = file_data();
  std::ofstream ofs(filename, std::ios::binary);
  ofs.write(reinterpret_cast<char const*>(data.data()), data.size());
}

std::vector<uint8_t> Image::file_header() const {
  std::vector<uint8_t> result{};
  result.reserve(14);
  result.push_back(static_cast<uint8_t>('B'));
  result.push_back(static_cast<uint8_t>('M'));
  vec_concat_inplace(result, get_le_bytes(54 + get_size() * 3));
  for (int i = 0; i < 4; i++) { result.push_back(static_cast<uint8_t>(0)); }
  vec_concat_inplace(result, get_le_bytes(54));
  return result;
}

std::vector<uint8_t> Image::bmp_header() const {
  std::vector<uint8_t> result{};
  result.reserve(40);
  vec_concat_inplace(result, get_le_bytes(static_cast<uint32_t>(40)));
  vec_concat_inplace(result, get_le_bytes(static_cast<uint32_t>(width)));
  vec_concat_inplace(result, get_le_bytes(static_cast<uint32_t>(height)));
  vec_concat_inplace(result, get_le_bytes(static_cast<uint16_t>(1)));
  vec_concat_inplace(result, get_le_bytes(static_cast<uint16_t>(24)));
  for (int i = 0; i < 24; i++) { result.push_back(static_cast<uint8_t>(0)); }
  return result;
}

std::vector<uint8_t> Image::image_data() const {
  std::vector<uint8_t> result{};
  auto padding = (4 - ((width * 3) % 4)) % 4;
  result.reserve(get_size() * 3 + padding * height);

  // TODO: MISSING LAST LINE
  for (auto y = height - 1;; --y) {
    if (y >= height) break;
    for (auto x = 0; x < width; ++x) {
      auto const& c = get_pixel(x, y);
      result.push_back(c.blue);
      result.push_back(c.green);
      result.push_back(c.red);
    }
    for (auto i = 0; i < padding; ++i) { result.push_back(static_cast<uint8_t>(0)); }
  }
  return result;
}

std::vector<uint8_t> Image::file_data() const {
  std::vector<uint8_t> result{};
  auto fheader = file_header();
  auto bheader = bmp_header();
  auto data = image_data();
  result.reserve(fheader.size() + bheader.size() + data.size());
  vec_concat_inplace(result, fheader);
  vec_concat_inplace(result, bheader);
  vec_concat_inplace(result, data);
  return result;
}

std::fstream& operator<<(std::fstream& os, Image const& img) {
  auto data = img.file_data();
  os.write(reinterpret_cast<char const*>(data.data()), data.size());
  return os;
}

std::fstream& operator<<(std::fstream& os, Close) {
  os.close();
  return os;
}
