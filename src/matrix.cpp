#include <sstream>
#include <iostream>
#include <string>
#include <array>
#include <cmath>
#include <limits>
/*
 * matrix.cpp
 *
 *  Created on: 5 Aug 2016
 *      Author: KSD
 */
template<typename T,std::size_t Rows, std::size_t Columns, int Zero, int One>
class Matrix {
private:
	std::array<T, Rows * Columns> values;
	std::size_t m = Rows;
	std::size_t n = Columns;
public:
	static T zero(){return Zero;}
	static T one(){return One;}

	static Matrix<T, Rows, Columns, Zero, One> identity() {
		std::array<T, Rows * Columns> _values;
		_values.fill(Zero);
		int i = 0, j = 0;
		//fill the diagonal with 1s
		while (i < Rows && j < Columns) {
			_values[i * Columns + j] = One;
			i++;
			j++;
		}
		return Matrix<T, Rows, Columns, Zero, One>(_values);
	}

	/**
	 * \brief Create a m rows by n columns matrix with every value set to zero.
	 */
	Matrix() {
		values.fill(zero());
	}

	/**
	 * \brief Create a m rows by n columns matrix with every value set to the fillValue.
	 * \param fillValue The value to initialize all the values in the matrix with.
	 */
	Matrix(const T& fillValue) {
		values.fill(fillValue);
	}

	/**
	 * \brief Create a m by n matrix whose values are set to the ones provided in the values array.
	 * \param value The value to initialize all the values in the matrix with.
	 */
	Matrix(std::array<T, Rows * Columns> const &_values) : values(_values) {
	}

	typename std::array<T, Rows * Columns>::const_iterator begin() const{
		return values.cbegin();
	}


	typename std::array<T, Rows * Columns>::const_iterator end() const{
		return values.cend();
	}

	T setValue(int row, int column, const T& val) {
		T oldValue = values[(row - 1) * Columns + (column - 1)];
		values[(row - 1) * Columns + (column - 1)] = val;
		return oldValue;
	}

	const T& getValue(int row, int column) const {
		return values[(row - 1) * Columns + (column - 1)];
	}

	const std::size_t& getRowsCount() const{
		return m;
	}

	const std::size_t& getColumnsCount() const{
		return n;
	}

	std::string toString() const{
		std::ostringstream matrix;
		for (int i = 0; i < m; i++) {
			matrix << "[  ";
			for (int j = 0; j < n; j++) {
				matrix << values[i * n + j] << "  ";
			}
			matrix << "]\n";
		}

		return matrix.str();
	}

	//Overloaded binary operators
	Matrix<T, Rows, Columns, Zero, One> operator+(Matrix<T, Rows, Columns, Zero, One> const &B) {
		std::array<T, Rows * Columns> _values;
		int index = 0;
		for (int i = 0; i < Rows; i++) {

			for (int j = 0; j < Columns; j++) {
				index = i * Columns + j;
				_values[index] = values[index] + B.getValue(i + 1, j + 1);
			}

		}
		return Matrix<T, Rows, Columns, Zero, One>(_values);
	}

	Matrix<T, Rows, Columns, Zero, One> operator-(const Matrix<T, Rows, Columns, Zero, One>& B) {
		std::array<T, Rows * Columns> _values;
		int index = 0;
		for (int i = 0; i < Rows; i++) {

			for (int j = 0; j < Columns; j++) {
				index = i * Columns + j;
				_values[index] = values[index] - B.getValue(i + 1, j + 1);
			}

		}
		return Matrix<T, Rows, Columns, Zero, One>(_values);
	}

	//scalar multiplication
	friend Matrix<T, Rows, Columns, Zero, One> operator*(const T& scalar,
			Matrix<T, Rows, Columns, Zero, One> const &A) {
		std::array<T, Rows * Columns> _values;

		for (int i = 0; i < Rows; i++) {
			for (int j = 0; j < Columns; j++) {
				_values[i * Columns + j] = A.getValue(i + 1, j + 1) * scalar;
			}

		}
		return Matrix<T, Rows, Columns, Zero, One>(_values);
	}
	//is commutative
	friend Matrix<T, Rows, Columns, Zero, One> operator*(Matrix<T, Rows, Columns, Zero, One> const &A,
			const T& scalar) {
		return scalar * A;
	}
	//multiplication by a scalar and mutation
	void operator*=(const T& scalar) {
		for (auto it = values.begin(); it != values.end(); it++) {
			*it *= scalar;
		}
	}

	//Division by a scalar
	friend Matrix<T, Rows, Columns, Zero, One> operator/(Matrix<T, Rows, Columns, Zero, One> const &A,
			const T& scalar) {
		return A * (one()/scalar);
	}
	//division by a scalar and mutation
	void operator/=(const T& scalar) {
		*this *= (one()/scalar);
	}


	//Matrix multiplication
	template <std::size_t aRows, std::size_t bColumns, std::size_t common>
	friend Matrix<T, aRows, bColumns, Zero, One> operator*( Matrix<T, aRows, common, Zero, One> const &A, Matrix<T, common, bColumns, Zero, One> const &B) {
		std::array<T, aRows * bColumns> _values;
		int index = Zero;

		for (int i = 0; i < aRows; i++) {
			for (int k = 0; k < bColumns; k++) {
				_values[index] = 0;
				for (int j = 0; j < common; j++) {
					_values[index] += A.getValue(i + 1, j + 1) * B.getValue(j + 1, k + 1);
				}
				index++;
			}
		}
		return Matrix<T, aRows, bColumns, Zero, One>(_values);
	}

	template <std::size_t aRows, std::size_t aColumns, std::size_t bRows, std::size_t bColumns>
	friend bool operator==(const Matrix<T, aRows, aColumns, Zero, One>& A, const Matrix<T, bRows, bColumns, Zero, One>& B) {
			std::cout << "Exact comparison" << "\n";
			if( aRows != bRows || aColumns != bColumns ){
				return false;
			}

			auto iteratorA = A.begin();
			auto iteratorB = B.begin();
			while( iteratorA != A.end() && iteratorB != B.end() ){
				if( *iteratorA != *iteratorB ){
					return false;
				}
				iteratorA++;
				iteratorB++;
			}

			return true;
		}

	template <std::size_t aRows, std::size_t aColumns, std::size_t bRows, std::size_t bColumns>
	friend bool operator!=(const Matrix<T, aRows, aColumns, Zero, One>& A, const Matrix<T, bRows, bColumns, Zero, One>& B) {
		return !( A == B );
	}
};

//Matrices holding integers
template<typename T, std::size_t Rows, std::size_t Columns>
using IntegerTypeMatrix = Matrix< typename std::enable_if<std::is_integral<T>::value, T>::type, Rows, Columns, 0, 1>;

template<std::size_t Rows, std::size_t Columns>
using IntMatrix = Matrix<int, Rows, Columns, 0, 1>;



/**
 * Matrices holding floating points.
 * Handles floating points value comparison.
 */
template<typename T, std::size_t Rows, std::size_t Columns, int Zero, int One>
class FloatingPointMatrix: public Matrix<
		typename std::enable_if<std::is_floating_point<T>::value, T>::type,
		Rows, Columns, Zero, One> {

		public:
			static T zero(){return (T)0.0;}
			static T one(){return (T)1.0;}

			using Matrix<T,Rows, Columns, Zero,One>::Matrix;
			FloatingPointMatrix<T, Rows, Columns, Zero, One> (const Matrix<T, Rows, Columns, Zero, One> x) {}
			FloatingPointMatrix<T, Rows, Columns, Zero, One>& operator= (const Matrix<T, Rows, Columns, Zero, One>& x) {return *this;}
			operator Matrix<T, Rows, Columns, Zero, One>() {return Matrix<T, Rows, Columns, Zero, One>();}

	template<std::size_t aRows, std::size_t aColumns, std::size_t bRows,
			std::size_t bColumns>
	friend bool operator==(const FloatingPointMatrix<T, aRows, aColumns, Zero, One>& A,
			const FloatingPointMatrix<T, bRows, bColumns, Zero, One>& B) {
		if (aRows != bRows || aColumns != bColumns) {
			return false;
		}

		auto iteratorA = A.begin();
		auto iteratorB = B.begin();
		while (iteratorA != A.end() && iteratorB != B.end()) {
			if (std::abs(*iteratorA - *iteratorB) > std::numeric_limits<T>::epsilon()) {
				return false;
			}
			iteratorA++;
			iteratorB++;
		}

		return true;
	}

	template <std::size_t aRows, std::size_t aColumns, std::size_t bRows, std::size_t bColumns>
	friend bool operator!=(const FloatingPointMatrix<T, aRows, aColumns, Zero, One>& A, const FloatingPointMatrix<T, bRows, bColumns, Zero, One>& B) {
		return !( A == B );
	}

	//Multiplication by a scalar
	friend FloatingPointMatrix<T, Rows, Columns, Zero, One> operator*(const T& scalar,
			FloatingPointMatrix<T, Rows, Columns, Zero, One> const &A) {
		std::array<T, Rows * Columns> _values;

		for (int i = 0; i < Rows; i++) {
			for (int j = 0; j < Columns; j++) {
				_values[i * Columns + j] = A.getValue(i + 1, j + 1) * scalar;
			}

		}
		return FloatingPointMatrix<T, Rows, Columns, Zero, One>(_values);
	}

	//is commutative
	friend FloatingPointMatrix<T, Rows, Columns, Zero, One> operator*(FloatingPointMatrix<T, Rows, Columns, Zero, One> const &A,
			const T& scalar) {
		return scalar * A;
	}

	//Division by a scalar
	friend FloatingPointMatrix<T, Rows, Columns, Zero, One> operator/(FloatingPointMatrix<T, Rows, Columns, Zero, One> const &A,
			const T& scalar) {
		return A * (one()/scalar);
	}
};

template<std::size_t Rows, std::size_t Columns>
using FloatMatrix = FloatingPointMatrix<float, Rows, Columns, 0, 1>;

template<std::size_t Rows, std::size_t Columns>
using DoubleMatrix = FloatingPointMatrix<double, Rows, Columns, 0, 1>;

template<std::size_t Rows, std::size_t Columns>
using LongDoubleMatrix = FloatingPointMatrix<long double, Rows, Columns, 0, 1>;
//Matrices of matrices
/*
 * TODO: Read more about template and meta programming
*/
/*
template<typename T, std::size_t Rows, std::size_t Columns>
class MatricesMatrix : public Matrix<MatricesMatrix, Rows, Columns, T, T>{
};
*/
