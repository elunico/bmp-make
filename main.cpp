#include "color.h"
#include "image.h"
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

std::ostream &operator<<(std::ostream &os, uint8_t const &c) {
  os << static_cast<int>(c);
  return os;
}

std::map<char, Color> build_map(std::vector<char> const &chars) {
  std::map<char, Color> map{};
  auto max = chars.size() - 1;
  for (auto i = 0; i < chars.size(); ++i) {
    uint8_t value = static_cast<uint8_t>((i / static_cast<double>(max)) * 255);
    auto color = Color{value, value, value};
    map.insert({chars[i], color});
  }
  return map;
}

std::vector<std::vector<char>> tokenize(std::vector<char> const &str,
                                        const char delim) {
  std::vector<std::vector<char>> tokens{};
  auto start = 0;
  while (start < str.size()) {
    auto end = start;
    while (end < str.size() && str[end] != delim) {
      ++end;
    }
    tokens.push_back(std::vector<char>(str.begin() + start, str.begin() + end));
    start = end + 1;
  }
  return tokens;
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

  std::cerr << "\033[94m"
            << "Reading file " << argv[2] << std::endl;
  auto filename = std::string{argv[2]};

  std::cerr << "\033[94m"
            << "Read file!" << std::endl;
  auto map = build_map(chars);

  std::cerr << "\033[94m"
            << "Reading stdin..." << std::endl;
  std::vector<char> image_content{};
  char c;
  while (std::cin.get(c)) {
    image_content.push_back(c);
  }

  std::cerr << "\033[94m"
            << "Done reading stdin" << std::endl;

  std::cerr << "\033[94m"
            << "Tokenizing lines..." << std::endl;
  std::vector<std::vector<char>> lines = tokenize(image_content, '\n');
  std::cerr << "\033[94m"
            << "Found " << lines.size() << " lines" << std::endl;

  if (lines.size() == 0 || lines[0].size() == 0) {
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
  for (auto const &line : lines) {
    auto x = 0;
    for (auto const &c : line) {
      Color clr;
      try {
        clr = map.at(c);
      } catch (...) {
        std::cerr << "\033[94m"
                  << "Invalid character: " << c << " substituting black\n";
        clr = Color{0, 0, 0};
      }

      image.set_pixel(x, y, clr);
      x++;
    }
    y++;
  }
  std::cerr << "\033[94m"
            << "Writing file..." << std::endl;
  image.write_to_file(filename);
}
