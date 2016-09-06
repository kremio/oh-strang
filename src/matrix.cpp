#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <stdexcept>

/*
 * matrix2.cpp
 *
 *  Created on: 8 Aug 2016
 *      Author: KSD
 */

/*
 * Values comparator
 */

template<typename T, typename Comparator>
class Comparator_Operator {
public:
	int operator()(const T& a, const T& b) {
		return Comparator::compare(a, b);
	}
};

template<typename T>
class DefaultComparator {
public:
	static int compare(const T& a, const T& b) {
		return a > b ? 1 : (a == b ? 0 : -1);
	}
};

template<typename T>
class FloatingPointComparator {
public:
	static int compare(const T& a, const T& b) {
		if (std::abs(a - b) < std::numeric_limits<T>::epsilon()) {
			return 0;
		}
		return a > b ? 1 : -1;
	}
};

template<typename T>
class Comparator: public Comparator_Operator<T,
		typename std::conditional<std::is_floating_point<T>::value,
				FloatingPointComparator<T>, DefaultComparator<T>>::type> {
};

template<typename T>
class Matrix {
protected:
	std::vector<T> values;
	std::size_t m;
	std::size_t n;
	T zero;
	T one;
protected:
	static Comparator<T> compare;

public:
	static Matrix<T> identity(const std::size_t& rows,
			const std::size_t& columns, const T& z0, const T& o1) {
		Matrix<T> I(rows, columns, z0, o1);
		for (int i = 1; i <= rows; i++) {
			for (int j = 1; j <= columns; j++) {
				if (j == i) {
					I.setValue(i, j, o1);
				}
			}
		}
		return I;
	}

	//Constructors
	Matrix():m(0), n(0), zero(0), one(1){}

	Matrix(const std::size_t& rows, const std::size_t& columns, const T& z0,
			const T& o1) :
			m(rows), n(columns), zero(z0), one(o1) {
		m = rows;
		n = columns;
		values.assign(m * n, zero);
	}

	Matrix(const std::size_t& rows, const std::size_t& columns, const T& z0,
			const T& o1, const T& fillValue) :
			Matrix(rows, columns, fillValue, o1) {
		zero = z0;
	}

	Matrix(std::size_t rows, std::size_t columns, T const &z0, T const &o1,
			T* _values) :
			Matrix(rows, columns, z0, o1) {
		//convert to vector
		std::vector<T> tmpVec(&_values[0], &_values[m * n]);
		//and copy
		values.insert(values.begin(), tmpVec.begin(), tmpVec.end());
	}

	//Setters
	T setValue(int row, int column, const T& val) {
		T oldValue = values[(row - 1) * n + (column - 1)];
		values[(row - 1) * n + (column - 1)] = val;
		return oldValue;
	}

	//Getters
	const T& getValue(int row, int column) const {
		return values[(row - 1) * n + (column - 1)];
	}

	const std::size_t& getRowsCount() const {
		return m;
	}

	const std::size_t& getColumnsCount() const {
		return n;
	}

	const T& getZero() const{
		return zero;
	}

	const T& getOne() const{
		return one;
	}

	T* getValues(){
		return &values[0];
	}

	//Iterators
	typename std::vector<T>::const_iterator begin() const {
		return values.cbegin();
	}

	typename std::vector<T>::const_iterator end() const {
		return values.cend();
	}

	//casting
	std::string toString() const {
		std::ostringstream matrix;
		for (int i = 0; i < m; i++) {
			matrix << "[  ";
			for (int j = 0; j < n; j++) {
				matrix << values[i * n + j] << "  ";
			}
			matrix << "]" << ((i < m - 1) ? "\n" : "");
		}

		return matrix.str();
	}

	//Transpose
	Matrix<T> transpose(){
		T _values[m * n];
		int index = 0;
		for (int i = 0; i < m; i++) {

			for (int j = 0; j < n; j++) {
				index = j * m + i;
				_values[index] = getValue(i + 1, j + 1);
			}

		}
		return Matrix<T>(n, m, zero, one, _values);
	}

	//Swap rows
	Matrix<T> swapRows(int rowA, int rowB){
		if( rowA < 1 || rowA > m || rowB < 1 || rowB > m ){
			throw std::out_of_range("Row index must be between 1 and rowsCount()");
		}

		rowA--;
		rowB--;

		Matrix<T> swapped(m,n, zero, one, getValues() );


		if( rowA == rowB ){
			return swapped;
		}

		T* _values = swapped.getValues();
		T temp;
		for(int i = 0; i < n; i++){
			temp = _values[rowB * n + i];
			_values[rowB * n + i] = _values[rowA * n + i];
			_values[rowA * n + i] = temp;
		}
		return swapped;
	}

	//Swap columns
	Matrix<T> swapColumns(int colA, int colB) {
		if (colA < 1 || colA > n || colB < 1 || colB > n) {
			throw std::out_of_range(
					"Column index must be between 1 and columnsCount()");
		}

		colA--;
		colB--;

		Matrix<T> swapped(m, n, zero, one, getValues());

		if (colA == colB) {
			return swapped;
		}

		T* _values = swapped.getValues();
		T temp;
		for (int i = 0; i < m; i++) {
			temp = _values[i * n + colB];
			_values[i * n + colB] = _values[i * n + colA];
			_values[i * n + colA] = temp;
		}
		return swapped;
	}

	//Concatenate the columns of a given matrix to this instance
	Matrix<T> concat(const Matrix<T>& B) {
		if( B.getRowsCount() !=  m){
			throw std::domain_error("Rows count must match.");
		}

		std::size_t _n = n + B.getColumnsCount();

		T _values[m * _n];
		for(int r = 0; r < m; r++){
			for(int c = 0; c < _n; c++ ){
				_values[r * _n + c ] = c < n ? values[r * n + c] : B.getValue(r + 1, c + 1 - n);
			}
		}

		return Matrix<T>(m, _n, zero, one, _values);
	}

	//Split the matrix into 2.
	//left points to the part of the matrix left of the splitColumn (included), right points to the part of the matrix right of the splitColumn (excluded)
	void split(int splitColumn, Matrix<T>& left, Matrix<T>& right){
		if( splitColumn < 1 || splitColumn >= n){
			throw std::out_of_range(
								"Split Column index must in the range ] 1; columnsCount() [");
		}

		T leftValues[m * splitColumn];

		T rightValues[m * (n - splitColumn)];

		for(int r = 0; r < m; r++){
			for( int c = 0; c < n; c++){
				if( c < splitColumn ){
					leftValues[r * splitColumn + c] = values[r * n + c];
				}else{
					rightValues[r * (n-splitColumn) + c - splitColumn] = values[r * n + c];
				}
			}
		}

		left = Matrix<T>(m, splitColumn, zero, one, leftValues);
		right = Matrix<T>(m, n - splitColumn, zero, one, rightValues);
	}

	//Equality operator
	friend bool operator==(const Matrix<T>& A, const Matrix<T>& B) {

		if (A.getRowsCount() != B.getRowsCount()
				|| A.getColumnsCount() != B.getColumnsCount()) {
			return false;
		}

		auto iteratorA = A.begin();
		auto iteratorB = B.begin();
		while (iteratorA != A.end() && iteratorB != B.end()) {
			if (Matrix<T>::compare(*iteratorA, *iteratorB) != 0) {
				return false;
			}
			iteratorA++;
			iteratorB++;
		}

		return true;
	}

	//Inequality operator
	friend bool operator!=(const Matrix<T>& A, const Matrix<T>& B) {
		return !(A == B);
	}

	//Multiplication by a scalar
	friend Matrix<T> operator*(const T& scalar, Matrix<T> const &A) {
		T _values[A.getRowsCount() * A.getColumnsCount()];
		int i = 0;
		for(auto it = A.begin(); it != A.end(); it++){
			_values[i++] = *it * scalar;
		}


		return Matrix<T>(A.getRowsCount(), A.getColumnsCount(), A.getZero(), A.getOne(), _values);
	}


	//is commutative
	friend Matrix<T> operator*(Matrix<T> const &A, const T& scalar) {
		return scalar * A;
	}

	//multiplication by a scalar and mutation
	void operator*=(const T& scalar) {
		for (auto it = values.begin(); it != values.end(); it++) {
			*it *= scalar;
		}
	}

	//Division by a scalar
	friend Matrix<T> operator/(Matrix<T> const &A,
			const T& scalar) {
		return A * (A.getOne()/scalar);
	}
	//division by a scalar and mutation
	void operator/=(const T& scalar) {
		*this *= (one/scalar);
	}

	//Overloaded binary operators
	Matrix<T> operator+(
			Matrix<T> const &B) {
		int Rows = B.getRowsCount();
		int Columns = B.getColumnsCount();

		if (m != Rows || n != Columns) {
			throw std::domain_error("Rows and columns count must match.");
		}


		T _values[Rows * Columns];
		int index = 0;
		for (int i = 0; i < Rows; i++) {

			for (int j = 0; j < Columns; j++) {
				index = i * Columns + j;
				_values[index] = values[index] + B.getValue(i + 1, j + 1);
			}

		}
		return Matrix<T>(Rows, Columns, B.getZero(), B.getOne(), _values);
	}

	Matrix<T> operator-(Matrix<T> const &B) {
		int Rows = B.getRowsCount();
		int Columns = B.getColumnsCount();

		if (m != Rows || n != Columns) {
			throw std::domain_error("Rows and columns count must match.");
		}

		T _values[Rows * Columns];
		int index = 0;
		for (int i = 0; i < Rows; i++) {

			for (int j = 0; j < Columns; j++) {
				index = i * Columns + j;
				_values[index] = values[index] - B.getValue(i + 1, j + 1);
			}

		}
		return Matrix<T>(Rows, Columns, B.getZero(), B.getOne(), _values);
	}

	//Matrix multiplication
	friend Matrix<T> operator*(Matrix<T> const &A, Matrix<T> const &B) {

		int aColumns = A.getColumnsCount();
		int bRows = B.getRowsCount();

		if (aColumns != bRows) {
			throw std::domain_error(
					"Left matrix columns count must match right matrix rows count.");
		}

		int aRows = A.getRowsCount();
		int bColumns = B.getColumnsCount();

		T _values[aRows * bColumns];
		int index = 0;

		for (int i = 0; i < aRows; i++) {
			for (int k = 0; k < bColumns; k++) {
				_values[index] = 0;
				for (int j = 0; j < aColumns; j++) {
					_values[index] += A.getValue(i + 1, j + 1) * B.getValue(j + 1, k + 1);
				}
				index++;
			}
		}
		return Matrix<T>(aRows, bColumns, A.getZero(), A.getOne(), _values);
	}

};

template<typename T>
std::basic_ostream<char>&
operator<<(std::basic_ostream<char>& __os, const Matrix<T>& A)
{
	const std::string __str = A.toString();
    return _VSTD::__put_character_sequence(__os, __str.c_str(), __str.length());
}

 /* JavaScript numbers are always stored as double precision floating point numbers, following the international IEEE 754 standard */
class DoubleMatrix : public Matrix<double> {
	public:
		static DoubleMatrix identity(const std::size_t& rows, const std::size_t& columns) {
			return DoubleMatrix( rows, columns, Matrix<double>::identity( rows, columns, 0, 1 ).getValues() );
		}

		DoubleMatrix(): Matrix<double>(0, 0, 0, 1){}

		DoubleMatrix(const std::size_t& rows, const std::size_t& columns) : Matrix<double>(rows, columns, 0, 1){}

		DoubleMatrix(const std::size_t& rows, const std::size_t& columns, const double& fillValue) : Matrix<double>(rows, columns, 0, 1, fillValue){}

		DoubleMatrix(std::size_t rows, std::size_t columns, double* _values) : Matrix<double>(rows, columns, 0, 1, _values) {}

		DoubleMatrix scalarMul(double scalar) { return DoubleMatrix( m, n, (*this * scalar).getValues() ); }
		DoubleMatrix matrixMul(DoubleMatrix matrix) { return DoubleMatrix( m, matrix.getColumnsCount(), (*this * matrix).getValues() ); };

};

