#include <sstream>
#include <iostream>
#include <stdio.h>
#include <string>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "matrix.cpp"


int main() {
  //And a little bit of C++, leaving together, in harmony
  Matrix<int, 3, 3> mat = Matrix<int, 3, 3>::identity(0,1);
  std::cout << (mat + mat).toString() << (mat - mat - mat).toString() << (999 * mat * 666).toString();

  Matrix<int, 3, 3> A = Matrix<int, 3, 3>(1);
  Matrix<int, 3, 4> B = Matrix<int, 3, 4>(1);

  std::cout << "\n\n" << A.toString() << " *\n" << B.toString() << " =\n" << (A * B).toString();
  return 0;
}
