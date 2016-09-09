#include "../src/matrix.cpp"

/*
 * 'Wrappers' to interface JS and the matrix library.
 */

/* JavaScript numbers are always stored as double precision floating point numbers, following the international IEEE 754 standard */
class DoubleMatrix : public MatrixCRTP<double, DoubleMatrix> {
	using MatrixCRTP<double, DoubleMatrix>::MatrixCRTP;

	public:
		static DoubleMatrix identity(const std::size_t& rows, const std::size_t& columns) {
			return DoubleMatrix( rows, columns, Matrix<double>::identity( rows, columns, 0, 1 ).getValues() );
		}

		DoubleMatrix(): MatrixCRTP<double, DoubleMatrix>(0, 0, 0, 1){}

		DoubleMatrix(const std::size_t& rows, const std::size_t& columns) : MatrixCRTP<double, DoubleMatrix>(rows, columns, 0, 1){}

		DoubleMatrix(const std::size_t& rows, const std::size_t& columns, const double& fillValue) : MatrixCRTP<double, DoubleMatrix>(rows, columns, 0, 1, fillValue){}

		DoubleMatrix(std::size_t rows, std::size_t columns, double* _values) : MatrixCRTP<double, DoubleMatrix>(rows, columns, 0, 1, _values) {}

		DoubleMatrix scalarMul(double scalar) { return DoubleMatrix( m, n, (*this * scalar).getValues() ); }
		DoubleMatrix matrixMul(DoubleMatrix matrix) { return DoubleMatrix( m, matrix.getColumnsCount(), (*this * matrix).getValues() ); };


		const char* asString(){
			return toString().c_str();
		}

};




