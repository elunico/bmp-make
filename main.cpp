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

std::map<char, Color> build_map(std::vector<char> &chars) {
  auto map = std::map<char, Color>();
  auto max = chars.size() - 1;
  for (auto i = 0; i < chars.size(); ++i) {
    uint8_t value = static_cast<uint8_t>((i / static_cast<double>(max)) * 255);
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
