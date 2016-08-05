#include <sstream>
#include <string>
#include <array>
/*
 * matrix.cpp
 *
 *  Created on: 5 Aug 2016
 *      Author: KSD
 */
template<typename T, std::size_t Rows, std::size_t Columns>
class Matrix {
private:
	std::array<T, Rows * Columns> values;
public:
	static Matrix<T, Rows, Columns> identity(const T& zero, const T& one) {
		std::array<T, Rows * Columns> _values;
		_values.fill(zero);
		int i = 0, j = 0;
		//fill the diagonal with 1
		while (i < Rows && j < Columns) {
			_values[i * Columns + j] = one;
			i++;
			j++;
		}
		return Matrix<T, Rows, Columns>(_values);
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
	Matrix(const std::array<T, Rows * Columns> _values) :
			values(_values) {
	}
	;

	T setValue(int row, int column, const T& val) {
		T oldValue = values[(row - 1) * Columns + (column - 1)];
		values[(row - 1) * Columns + (column - 1)] = val;
		return oldValue;
	}

	const T& getValue(int row, int column) const {
		return values[(row - 1) * Columns + (column - 1)];
	}

	const std::size_t& getRowsCount() const{
		return Rows;
	}

	const std::size_t& getColumnsCount() const{
		return Columns;
	}

	std::string toString() {
		std::ostringstream matrix;
		for (int i = 0; i < Rows; i++) {
			matrix << "[  ";
			for (int j = 0; j < Columns; j++) {
				matrix << values[i * Columns + j] << "  ";
			}
			matrix << "]\n";
		}

		return matrix.str();
	}

	//Overloaded binary operators
	Matrix<T, Rows, Columns> operator+(const Matrix<T, Rows, Columns>& B) {
		std::array<T, Rows * Columns> _values;
		int index = 0;
		for (int i = 0; i < Rows; i++) {

			for (int j = 0; j < Columns; j++) {
				index = i * Columns + j;
				_values[index] = values[index] + B.getValue(i + 1, j + 1);
			}

		}
		return Matrix<T, Rows, Columns>(_values);
	}

	Matrix<T, Rows, Columns> operator-(const Matrix<T, Rows, Columns>& B) {
		std::array<T, Rows * Columns> _values;
		int index = 0;
		for (int i = 0; i < Rows; i++) {

			for (int j = 0; j < Columns; j++) {
				index = i * Columns + j;
				_values[index] = values[index] - B.getValue(i + 1, j + 1);
			}

		}
		return Matrix<T, Rows, Columns>(_values);
	}

	//scalar multiplication
	friend Matrix<T, Rows, Columns> operator*(const T& scalar,
			const Matrix<T, Rows, Columns>& A) {
		std::array<T, Rows * Columns> _values;

		for (int i = 0; i < Rows; i++) {
			for (int j = 0; j < Columns; j++) {
				_values[i * Columns + j] = A.getValue(i + 1, j + 1) * scalar;
			}

		}
		return Matrix<T, Rows, Columns>(_values);
	}
	friend Matrix<T, Rows, Columns> operator*(const Matrix<T, Rows, Columns>& A,
			const T& scalar) {
		return scalar * A;
	}

	//Matrix multiplication
	template <std::size_t aRows, std::size_t bColumns, std::size_t common> friend Matrix<T, aRows, bColumns> operator*(const Matrix<T, aRows, common>& A, const Matrix<T, common, bColumns>& B) {
		std::array<T, aRows * bColumns> _values;
		int index = 0;
		for (int i = 0; i < aRows; i++) {
			for (int k = 0; k < bColumns; k++) {
				index = i * common + k;
				_values[index] = 0;
				for (int j = 0; j < common; j++) {
					_values[index] += A.getValue(i + 1, j + 1) * B.getValue(j + 1, k + 1);
				}
			}
		}
		return Matrix<T, aRows, bColumns>(_values);
	}
};

