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

template<typename T, typename C>
class MatrixCRTP {
protected:
	std::vector<T> values;
	std::size_t m;
	std::size_t n;
	T zero;
	T one;
protected:
	static Comparator<T> compare;

public:
	static C identity(const std::size_t& rows,
			const std::size_t& columns, const T& z0, const T& o1) {
		C I(rows, columns, z0, o1);
		for (int i = 1; i <= rows; i++) {
			for (int j = 1; j <= columns; j++) {
				if (j == i) {
					I.setValue(i, j, o1);
				}
			}
		}
		return I;
	}

	static C getPermutationMatrix(int size, T zero, T one, int c1, int c2){

		C xchange = C::identity( size, size, zero, one);
		if( c1 == c2 ){
			return xchange;
		}

		if( c1 < 1 || c2 < 1 || c1 > size || c2 > size  ){
			std::ostringstream err;
			err << "Column indices (" << c1 << "," << c2 << ") must be between 1 and " << size;
			throw std::out_of_range( err.str() );
		}

		xchange.setValue(c1, c1, zero);
		xchange.setValue(c2, c2, zero);
		xchange.setValue(c1, c2, one);
		xchange.setValue(c2, c1, one);
		return xchange;
	}

	//Constructors
	MatrixCRTP():m(0), n(0), zero(0), one(1){}

	MatrixCRTP(const std::size_t& rows, const std::size_t& columns, const T& z0,
			const T& o1) :
			m(rows), n(columns), zero(z0), one(o1) {
		m = rows;
		n = columns;
		values.assign(m * n, zero);
	}

	MatrixCRTP(const std::size_t& rows, const std::size_t& columns, const T& z0,
			const T& o1, const T& fillValue) :
			MatrixCRTP(rows, columns, fillValue, o1) {
		zero = z0;
	}

	MatrixCRTP(std::size_t rows, std::size_t columns, T const &z0, T const &o1,
			T* _values) :
			MatrixCRTP(rows, columns, z0, o1) {
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
	C transpose(){
		T _values[m * n];
		int index = 0;
		for (int i = 0; i < m; i++) {

			for (int j = 0; j < n; j++) {
				index = j * m + i;
				_values[index] = getValue(i + 1, j + 1);
			}

		}
		return C(n, m, zero, one, _values);
	}

	//Swap rows
	C swapRows(int rowA, int rowB){
		if( rowA < 1 || rowA > m || rowB < 1 || rowB > m ){
			throw std::out_of_range("Row index must be between 1 and rowsCount()");
		}

		return C::getPermutationMatrix( m, zero, one, rowA, rowB ) * *static_cast<C*>(this);
	}

	//Swap columns
	C swapColumns(int colA, int colB) {
		if (colA < 1 || colA > n || colB < 1 || colB > n) {
			throw std::out_of_range(
					"Column index must be between 1 and columnsCount()");
		}

		return *static_cast<C*>(this) * C::getPermutationMatrix( n, zero, one, colA, colB );
	}

	//Concatenate the columns of a given matrix to this instance
	C concat(const C& B) {
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

		return C(m, _n, zero, one, _values);
	}

	//Split the matrix into 2.
	//left points to the part of the matrix left of the splitColumn (included), right points to the part of the matrix right of the splitColumn (excluded)
	void split(int splitColumn, C& left, C& right){
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

		left = C(m, splitColumn, zero, one, leftValues);
		right = C(m, n - splitColumn, zero, one, rightValues);
	}


	// Perform the L * U decomposition of the matrix.
	bool toLU( C& L, C& U ){

		bool singular = false;

		//Create an copy of this matrix
		U = C(m, n, zero, one, getValues());

		//This matrix will hold the multipliers
		L = C::identity( m, m, zero, one );

		//These matrices will store row exchange operations
		C rowExchange;
		C rowExchanges = C::identity( m, m, zero, one );


		//Starting from the 1st row, for each row except the last, find the first non zero value
		int rC = 0;
		T pivotValue = zero;
		T vU = zero;
		for(int pivot = 1; pivot < m; pivot++){
			rC = pivot;
			while( rC <= m && U.getValue(rC, pivot) == zero){
				rC++;
			}

			if( rC > m){ //no non-zero values for this pivot (the matrix
				singular = true;
				continue;
			}

			if( rC != pivot ){ //row exchange required
				rowExchange = C::getPermutationMatrix(m, zero, one, pivot, rC);
				U = rowExchange * U;
				rowExchanges =  rowExchange * rowExchanges;
				rC = pivot;
			}

			pivotValue = U.getValue(pivot, pivot);

			//For each row under the pivot, look for a non zero value
			for( int rU = pivot + 1; rU <= m; rU++ ){
				vU = U.getValue( rU, pivot );
				if( vU == zero ){
					continue;
				}

				//Compute the multiplier to turn vU into zero
				vU /= pivotValue;
				//store in L
				L.setValue( rU, pivot, vU );

				//Apply to the row
				for(int c = rC; c <= n; c++ ){
					U.setValue( rU, c, U.getValue(rU, c) - U.getValue(pivot, c) * vU );
				}
			}
		}

		//Apply the inverse of the row exchanges matrix to L
		L = rowExchanges.transpose() * L;

		//Check the last row for singularity
		return ( !singular && U.getValue(m, m) == zero )? true : singular;
	}

	//Calculate determinant
	T det(){
		if (m != n) {
			throw std::domain_error("Only a square matrix has a determinant.");
		}

		C L;
		C U;

		//Perform LU decomposition
		if( toLU( L, U ) ){ //The matrix is singular
			return zero;
		}

		T d = 1;

		for (int i = 1; i <= m; i++) {
				d *= U.getValue(i,i);
		}

		return d;
	}

	//Equality operator
	friend bool operator==(const C& A, const C& B) {

		if (A.getRowsCount() != B.getRowsCount()
				|| A.getColumnsCount() != B.getColumnsCount()) {
			return false;
		}

		auto iteratorA = A.begin();
		auto iteratorB = B.begin();
		while (iteratorA != A.end() && iteratorB != B.end()) {
			if (C::compare(*iteratorA, *iteratorB) != 0) {
				return false;
			}
			iteratorA++;
			iteratorB++;
		}

		return true;
	}

	//Inequality operator
	friend bool operator!=(const C& A, const C& B) {
		return !(A == B);
	}

	//Multiplication by a scalar
	friend C operator*(const T& scalar, C const &A) {
		T _values[A.getRowsCount() * A.getColumnsCount()];
		int i = 0;
		for(auto it = A.begin(); it != A.end(); it++){
			_values[i++] = *it * scalar;
		}


		return C(A.getRowsCount(), A.getColumnsCount(), A.getZero(), A.getOne(), _values);
	}


	//is commutative
	friend C operator*(C const &A, const T& scalar) {
		return scalar * A;
	}

	//multiplication by a scalar and mutation
	void operator*=(const T& scalar) {
		for (auto it = values.begin(); it != values.end(); it++) {
			*it *= scalar;
		}
	}

	//Division by a scalar
	friend C operator/(C const &A,
			const T& scalar) {
		return A * (A.getOne()/scalar);
	}
	//division by a scalar and mutation
	void operator/=(const T& scalar) {
		*this *= (one/scalar);
	}

	//Overloaded binary operators
	C operator+(
			C const &B) {
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
		return C(Rows, Columns, B.getZero(), B.getOne(), _values);
	}

	C operator-(C const &B) {
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
		return C(Rows, Columns, B.getZero(), B.getOne(), _values);
	}

	//Matrix multiplication
	friend C operator*(C const &A, C const &B) {

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
		return C(aRows, bColumns, A.getZero(), A.getOne(), _values);
	}

};

template<typename T, class C>
std::basic_ostream<char>&
operator<<(std::basic_ostream<char>& __os, const MatrixCRTP<T, C>& A)
{
	const std::string __str = A.toString();
    return _VSTD::__put_character_sequence(__os, __str.c_str(), __str.length());
};

template<typename T>
class Matrix : public MatrixCRTP<T, Matrix<T>>{
	using MatrixCRTP<T, Matrix<T>>::MatrixCRTP;
};
