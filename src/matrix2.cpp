#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
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
private:
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
			T _values[]) :
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

	//Iterators
	typename std::vector<T>::const_iterator begin() const {
		return values.cbegin();
	}

	typename std::vector<T>::const_iterator end() const {
		return values.cend();
	}

	//Transform
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

	//Ostream
	/*
	friend std::ostringstream operator<<(std::ostringstream os, const Matrix<T>& A){
		return os << A.toString();
	}
*/
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
};

template<typename T>
std::basic_ostream<char>&
operator<<(std::basic_ostream<char>& __os, const Matrix<T>& A)
{
	const std::string __str = A.toString();
    return _VSTD::__put_character_sequence(__os, __str.c_str(), __str.length());
}

