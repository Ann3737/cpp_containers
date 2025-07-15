#include <iostream>

#include "s21_vector.h"

using s21::vector;

int main() {
  vector<int> vec{1, 2, 3};

  std::cout << vec.at(0);
  std::cout << vec.at(1);
  std::cout << vec.at(2) << std::endl;

  auto it = vec.begin();
  // ++it;
  vec.insert_many(it, 1, 2, 3);
  for (auto i = 0; i != 6; ++i) {
    std::cout << vec.at(i);
  }
  return 0;
}