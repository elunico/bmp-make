#include <bitset>
#include <cmath>
#include <fstream>
#include <ios>
#include <iostream>
#include <sstream>
#include <vector>

// Integer square root (using binary search)
unsigned int isqrt(unsigned int y) {
  unsigned int L = 0;
  unsigned int M;
  unsigned int R = y + 1;

  while (L != R - 1) {
    M = (L + R) / 2;

    if (M * M <= y)
      L = M;
    else
      R = M;
  }

  return L;
}

constexpr auto transform_bin =
    [](std::vector<char> const &digits) -> std::vector<char> {
  auto result = std::vector<char>();
  for (auto &digit : digits) {
    auto count = 0;
    auto value = static_cast<unsigned int>(digit - '0');
    while (value > 0) {
      result.push_back('0' + (value & 1));
      value >>= 1;
      count++;
    }
    while (count < 4) {
      result.push_back('0');
      count++;
    }
  }
  return result;
};

constexpr auto transform_dec =
    [](std::vector<char> const &digits) -> std::vector<char> { return digits; };

int usage(std::string const &name) {
  std::cerr << "Usage: " << name << " <FORMAT> <OUTPUT_FILE> [ <WIDTH> ]"
            << std::endl;
  return 1;
}

int main(int argc, char const *argv[]) {
  if (argc < 2 || argv[1] == nullptr) {
    return usage(argv[0]);
  }
  auto filename = argv[1];
  std::ifstream ifs{filename};
  if (!ifs) {
    return usage(argv[0]);
  }

  std::vector<char> digits;
  char c;
  while (ifs.good() && !ifs.eof()) {
    ifs >> c;
    digits.push_back(c);
  }

  if (argc < 3 || argv[2] == nullptr) {
    return usage(argv[0]);
  }
  if (std::string(argv[2]) == "dec") {
    digits = transform_dec(digits);
  } else if (std::string(argv[2]) == "bin") {
    digits = transform_bin(digits);
  } else {
    return usage(argv[0]);
  }

  auto width = isqrt(digits.size());
  if (argc == 4) {
    width = std::stoi(argv[3]);
  }

  std::size_t index = 0;
  for (auto const &digit : digits) {
    if (index > 0 && index % width == 0) {
      std::cout << std::endl;
    } else {
      std::cout << digit;
    }
    index++;
  }
  return 0;
}
