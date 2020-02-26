#include <iostream>
#include "../Marble/include/M.hpp"

using namespace Marble;

int main(int argc, char **argv) {
  std::cout << "Hello World!" << std::endl;
  M a = encrypt(5);
  M b = 7;
  a += b;
  std::cout << "Computed enc(5)+7 = " << decrypt(a) << std::endl;

  M c = encrypt(5);
  M d = 7;
  M e = c + d;
  std::cout << "Computed enc(5)+7 = " << decrypt(e) << std::endl;
}