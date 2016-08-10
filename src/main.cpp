#include <sstream>
#include <iostream>
#include <stdio.h>
#include <string>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "matrix2.cpp"


int main() {
  //And a little bit of C++, leaving together, in harmony
  Matrix<int> A = Matrix<int>::identity(3,3,0,1);
  std::cout << A << "\n";

  Matrix<float> B(3,6,0,1.0, 3.14);
  std::cout << B << "\n";
  char buff[6] = {'a','b','c','d','e','f'};

  Matrix<char> C(2,3,'0','1', buff );
  std::cout << C << "\n";

  Matrix<float> D(3,6,0,1.0, 3.14);
  std::cout << (D == B ? "equal. ok!" : "equal broken") << "\n";

  std::cout << (A == A ? "equal. ok!" : "equal broken") << "\n";

  Matrix<char> charZero(2,3,'0','1');
  Matrix<Matrix<char>> matMat(3, 3, charZero, Matrix<char>::identity(2,3,0,1), C);
  std::cout << matMat << "\n";

  /*
  std::cout << (mat + mat).toString() << (mat - mat - mat).toString() << (999 * mat * 666).toString();

  Matrix<int, 3, 3> A = Matrix<int, 3, 3>(1);
  Matrix<int, 3, 6> B = Matrix<int, 3, 6>(1);

  std::cout << "\n\n" << A.toString() << " *\n" << B.toString() << " =\n" << (A * B).toString();
  */
  return 0;
}
