#include <iostream>
#include "G.hpp"

using namespace Granite;

int main(int argc, char **argv) {
  std::cout << "Hello World!" << std::endl;
  G a = encrypt(5);
  G b = 7;
  a += b;
  std::cout << "Computed enc(5)+7 = " << decrypt(a) << std::endl;

  G c = encrypt(5);
  G d = 7;
  G e = c + d;
  std::cout << "Computed enc(5)+7 = " << decrypt(e) << std::endl;
}