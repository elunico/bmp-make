#include <bitset>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

bool streql(char const* a, char const* b) {
  while (*a && *b) {
    if (*a != *b) { return false; }
    a++;
    b++;
  }
  return *a == *b;
}

// Integer square root (using binary search)
unsigned int isqrt(unsigned int const y) {
  unsigned int L = 0;
  unsigned int M;
  unsigned int R = y + 1;

  while (L != R - 1) {
    M = (L + R) / 2;
    if (M * M <= y) L = M;
    else
      R = M;
  }

  return L;
}

constexpr auto transform_bin = [](std::vector<char> const& digits) -> std::vector<char> {
  auto result = std::vector<char>();
  for (auto const& digit: digits) {
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

constexpr auto transform_dec = [](std::vector<char> const& digits) -> std::vector<char> { return digits; };

int usage(std::string const& name) {
  std::cerr << "\033[95m"
            << "Usage: " << name << " <INPUT_FILE> <FORMAT> [ <WIDTH> ]"
            << "\033[0m" << std::endl;
  return 1;
}

int main(int argc, char const* argv[]) {
  if (argc < 2 || argv[1] == nullptr) { return usage(argv[0]); }
  std::cerr << "\033[95m"
            << "Opening file " << argv[1] << "\033[0m" << std::endl;
  auto filename = argv[1];
  std::ifstream ifs{ filename };
  if (!ifs) {
    std::cerr << "\033[95m"
              << "Could not open file " << filename << "\033[0m" << std::endl;
    return usage(argv[0]);
  }

  std::cerr << "\033[95m"
            << "Reading file " << filename << "\033[0m" << std::endl;
  std::vector<char> digits;
  char c;
  while (ifs.good() && !ifs.eof()) {
    ifs >> c;
    digits.push_back(c);
  }
  std::cerr << "\033[95m"
            << "Found " << digits.size() << " digits"
            << "\033[0m" << std::endl;

  if (argc < 3 || argv[2] == nullptr) { return usage(argv[0]); }

  std::cerr << "\033[95m"
            << "Transforming digits using " << argv[2] << "\033[0m" << std::endl;
  if (streql(argv[2], "dec")) {
    digits = transform_dec(digits);
  } else if (streql(argv[2], "bin")) {
    digits = transform_bin(digits);
  } else {
    return usage(argv[0]);
  }

  auto width = isqrt(digits.size());
  if (argc == 4) {
    std::cerr << "\033[95m"
              << "Using provided width of " << argv[3] << "\033[0m" << std::endl;
    width = std::stoi(argv[3]);
  }

  std::cerr << "\033[95m"
            << "Writing out transformed image data of length " << digits.size() << "\033[0m" << std::endl;
  std::size_t index = 0;
  for (auto const& digit: digits) {
    if (index > 0 && index % width == 0) {
      std::cout << '\n';
    } else {
      std::cout << digit;
    }
    index++;
  }
  return 0;
}
