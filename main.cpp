#include "color.h"
#include "image.h"
#include "point.h"
#include <cstdint>
#include <iostream>
#include <map>
#include <vector>

template <typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T> const& c) {
  os << "[";
  for (auto i = 0; i < c.size(); ++i) {
    os << static_cast<int>(c[i]);
    if (i != c.size() - 1) { os << ", "; }
  }
  os << "]";
  return os;
}

std::map<char, Color> build_map(std::vector<char> const& chars) {
  std::map<char, Color> map{};
  auto max = chars.size() - 1;
  for (auto i = 0; i < chars.size(); ++i) {
    auto value = static_cast<uint8_t>((i / static_cast<double>(max)) * 255);
    auto color = Color{ value, value, value };
    map.insert({ chars[i], color });
  }
  return map;
}

std::vector<std::vector<char>> tokenize(std::vector<char> const& str, const char delim) {
  std::vector<std::vector<char>> tokens{};
  auto start = 0;
  while (start < str.size()) {
    auto end = start;
    while (end < str.size() && str[end] != delim) { ++end; }
    tokens.emplace_back(str.begin() + start, str.begin() + end);
    start = end + 1;
  }
  return tokens;
}

int main(int argc, char* argv[]) {
  if (argv[1] == nullptr || argv[2] == nullptr) {
    std::cout << "Usage: " << argv[0] << " format fileout" << std::endl;
    return 1;
  }

  std::vector<char> chars{};
  std::string arg1{ argv[1] };
  if (arg1 == "bin") {
    chars.push_back('0');
    chars.push_back('1');
  } else if (arg1 == "dec" || arg1 == "hex") {
    for (char const& c: "0123456789") { chars.push_back(c); }
    if (arg1 == "hex") {
      for (char const& c: "abcdef") { chars.push_back(c); }
    }
  } else {
    std::cout << "Invalid argument" << std::endl;
    return 1;
  }

  auto const map = build_map(chars);

  std::cerr << "\033[94m"
            << "Reading stdin..." << std::endl;
  std::vector<char> image_content{};
  char c;
  while (std::cin.get(c)) { image_content.push_back(c); }

  std::cerr << "\033[94m"
            << "Done reading stdin" << std::endl;

  std::cerr << "\033[94m"
            << "Tokenizing lines..." << std::endl;
  std::vector<std::vector<char>> lines = tokenize(image_content, '\n');
  std::cerr << "\033[94m"
            << "Found " << lines.size() << " lines" << std::endl;

  if (lines.empty() || lines[0].empty()) {
    std::cerr << "\033[94m"
              << "Input has no content" << std::endl;
    std::cerr << "\033[94m"
              << "Exiting!" << std::endl;
    return 2;
  }

  std::cerr << "\033[94m"
            << "Creating Image..." << std::endl;
  Image image(lines[0].size(), lines.size());
  auto y = 0;
  for (auto const& line: lines) {
    auto x = 0;
    for (auto const& c: line) {
      Color clr;
      try {
        clr = map.at(c);
      } catch (...) {
        std::cerr << "\033[94m"
                  << "Invalid character: " << c << " substituting black\n";
        clr = Color::Empty;
      }

      image.set_pixel(x, y, clr);
      x++;
    }
    y++;
  }
  std::cerr << "\033[94m"
            << "Writing file..." << std::endl;

  image.write_to_file(std::string{ argv[2] });
  std::fstream stream{ argv[2], std::ios::out | std::ios::binary };
  stream << image << CLOSE;
  stream.close();

  //  std::cout << image.file_data() << std::endl;

  int r = Color{ 0, 0, 0 };
  std::cout << r << std::endl;
}
