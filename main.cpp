#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

using u8 = std::byte;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

struct Color {
  u8 red, green, blue;

public:
  Color(u8 red, u8 green, u8 blue) : red(red), green(green), blue(blue) {}
  Color() = default;
};

std::ostream &operator<<(std::ostream &os, Color const &c) {
  os << "Color(" << static_cast<int>(c.red) << ", " << static_cast<int>(c.green)
     << ", " << static_cast<int>(c.blue) << ")";
  return os;
}

std::ostream &operator<<(std::ostream &os, u8 const &c) {
  os << static_cast<int>(c);
  return os;
}

template <typename Integral> std::vector<u8> get_le_bytes(Integral value) {
  std::vector<u8> result;
  result.reserve(sizeof(value));
  for (auto i = 0; i < sizeof(value); ++i) {
    result.push_back(static_cast<u8>(value & 0xFF));
    value >>= 8;
  }
  return result;
}

struct Image {
  u32 width, height;
  std::vector<Color> pixels;

public:
  Image(u32 width, u32 height)
      : width(width), height(height), pixels(width * height) {}

  Color const &get_pixel(u32 x, u32 y) const {
    try {
      return pixels.at(y * width + x);
    } catch (...) {
      std::cout << "Out of bounds: " << x << ", " << y << std::endl;
      throw std::out_of_range("Out of range");
    }
  }

  void set_pixel(u32 x, u32 y, Color const &c) { pixels[y * width + x] = c; }

  u32 get_width() const { return width; }
  u32 get_height() const { return height; }
  u32 get_size() const { return width * height; }

  void write_to_file(std::string const &filename) const {
    auto data = file_data();
    std::ofstream ofs(filename, std::ios::binary);
    ofs.write(reinterpret_cast<char const *>(data.data()), data.size());
  }

private:
  std::vector<u8> file_header() const {
    auto result = std::vector<u8>();
    result.reserve(14);
    result.push_back(static_cast<u8>('B'));
    result.push_back(static_cast<u8>('M'));
    auto p = get_le_bytes(54 + get_size() * 3);
    result.insert(result.end(), p.begin(), p.end());
    for (int i = 0; i < 4; i++) {
      result.push_back(static_cast<u8>(0));
    }
    p = get_le_bytes(54);
    result.insert(result.end(), p.begin(), p.end());
    return result;
  }

  std::vector<u8> bmp_header() const {
    auto result = std::vector<u8>();
    result.reserve(40);
    auto p = get_le_bytes(40);
    result.insert(result.end(), p.begin(), p.end());
    p = get_le_bytes(width);
    result.insert(result.end(), p.begin(), p.end());
    p = get_le_bytes(height);
    result.insert(result.end(), p.begin(), p.end());
    p = get_le_bytes(static_cast<u16>(1));
    result.insert(result.end(), p.begin(), p.end());
    p = get_le_bytes(static_cast<u16>(24));
    result.insert(result.end(), p.begin(), p.end());
    for (int i = 0; i < 24; i++) {
      result.push_back(static_cast<u8>(0));
    }
    return result;
  }

  std::vector<u8> image_data() const {
    auto result = std::vector<u8>();
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
        result.push_back(static_cast<u8>(0));
      }
    }
    return result;
  }

  std::vector<u8> file_data() const {
    auto result = std::vector<u8>();
    auto fheader = file_header();
    auto bheader = bmp_header();
    auto data = image_data();
    result.reserve(fheader.size() + bheader.size() + data.size());
    result.insert(result.end(), fheader.begin(), fheader.end());
    result.insert(result.end(), bheader.begin(), bheader.end());
    result.insert(result.end(), data.begin(), data.end());
    return result;
  }
};

std::map<char, Color> build_map(std::vector<char> &chars) {
  auto map = std::map<char, Color>();
  auto max = chars.size() - 1;
  for (auto i = 0; i < chars.size(); ++i) {
    u8 value = static_cast<u8>((i / static_cast<double>(max)) * 255);
    auto color = Color{value, value, value};
    map.insert({chars[i], color});
  }
  return map;
}

void tokenize(std::string const &str, const char delim,
              std::vector<std::string> &out) {
  auto start = 0;
  while (start < str.size()) {
    auto end = str.find(delim, start);
    if (end == std::string::npos) {
      end = str.size();
    }
    out.push_back(str.substr(start, end - start));
    start = end + 1;
  }
}

int main(int argc, char *argv[]) {

  if (argv[1] == nullptr || argv[2] == nullptr) {
    std::cout << "Usage: " << argv[0] << " format fileout" << std::endl;
    return 1;
  }

  std::vector<char> chars{};
  if (std::string{argv[1]} == "bin") {
    chars.push_back('0');
    chars.push_back('1');
  } else if (std::string{argv[1]} == "dec" || std::string{argv[1]} == "hex") {
    chars.push_back('0');
    chars.push_back('1');
    chars.push_back('2');
    chars.push_back('3');
    chars.push_back('4');
    chars.push_back('5');
    chars.push_back('6');
    chars.push_back('7');
    chars.push_back('8');
    chars.push_back('9');
    if (std::string{argv[1]} == "hex") {
      chars.push_back('a');
      chars.push_back('b');
      chars.push_back('c');
      chars.push_back('d');
      chars.push_back('e');
      chars.push_back('f');
    }
  } else {
    std::cout << "Invalid argument" << std::endl;
    return 1;
  }

  auto filename = std::string{argv[2]};

  auto map = build_map(chars);

  std::vector<char> image_content;
  char c;
  while (std::cin.get(c)) {
    image_content.push_back(c);
  }

  std::cout << "Done reading input" << std::endl;

  std::string s(image_content.begin(), image_content.end());

  // panic!("String length is {}", string.len());
  auto lines = std::vector<std::string>();
  tokenize(s, '\n', lines);
  std::cout << lines.size() << std::endl;

  if (lines.size() == 0 || lines[0].size() == 0) {
    std::cout << "Input has no content" << std::endl;
    return 2;
  }

  auto image = Image(lines[0].size(), lines.size());
  auto y = 0;
  for (auto const &line : lines) {
    auto x = 0;
    for (auto const &c : line) {
      auto clr = map.at(c);

      image.set_pixel(x, y, clr);
      x++;
    }
    y++;
  }
  image.write_to_file(filename);
}
