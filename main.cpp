#include <iostream>
#include "Octagon.h"
#include "test_octagon.h"

int main() {

  myTestingFunction();

  Octagon first = Octagon(10, 20, 10, 20, 10, 20, 10, 20);
  first.inflate(5);
  first.inflate(-5);
  for (int i = 1; i <= 8; ++i) {
    std::cout << first.limit(i) << std::endl;
  }
  std::cout << 5 * sqrt(2) << std::endl;
  return 0;
}

