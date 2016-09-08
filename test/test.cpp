/*
 * test.cpp
 *
 *  Created on: 6 Aug 2016
 *      Author: KSD
 */
#include "lest.hpp"
#include "../src/matrix.cpp"

#include <array>

using namespace std;

const lest::test specification[] =
{
    CASE( "Default constructor fill matrix with zeros" )
    {
    	Matrix<int> matrix(2, 2, 0, 1);
    	for( auto matrixValuesIterator = matrix.begin(); matrixValuesIterator != matrix.end(); matrixValuesIterator++)
    	{
    		EXPECT( 0 == *matrixValuesIterator);
    	}
    },

    CASE( "Constructor accepting a single value fills matrix with the value" )
    {
    	int myValue = -10;
    	Matrix<int> matrix(3,2, 0, 1, myValue);
    	for( auto matrixValuesIterator = matrix.begin(); matrixValuesIterator != matrix.end(); matrixValuesIterator++)
    	{
    		EXPECT( myValue == *matrixValuesIterator);
    	}
    },

    CASE( "Constructor accepting an array of m * n values fills matrix with the values" )
    {
    	std::array<int, 2*3 > myValues;
    	int i = -10;
    	for(auto it = myValues.begin(); it != myValues.end(); it++){
    		*it = i++;
    	}
    	Matrix<int> matrix(3, 2, 0, 1, myValues.data());
    	auto it = myValues.begin();
    	for( auto matrixValuesIterator = matrix.begin(); matrixValuesIterator != matrix.end() && it != myValues.end(); matrixValuesIterator++)
    	{
    		EXPECT( *it++ == *matrixValuesIterator );
    	}
    },

	CASE( "The identity() static function creates a matrix where each value equals zero except for the values on its diagonal" )
	{
    	auto Identity4by3 = Matrix<int>::identity(4,3,0,1);
    	for(int i = 1; i <= Identity4by3.getRowsCount(); i++){
        	for(int j = 1; j <= Identity4by3.getColumnsCount(); j++){
        		if( i == j ){
        			EXPECT( 1 == Identity4by3.getValue(i,j) );
        		}else{
        			EXPECT( 0 == Identity4by3.getValue(i,j) );
        		}
        	}
    	}
	},

	CASE( "Matrices equality and inequality" )
	{
		Matrix<int> A(6, 5, 0, 1, 1);
		Matrix<int> B(6, 5, 0, 1, 1);
		Matrix<int> C(5, 6, 0, 1, 1);
		Matrix<int> D(6, 5, 0, 1, -1);

		EXPECT( true == (A == B && B == A) );
		EXPECT( true == (!(A == C) && A != C) );
		EXPECT( B != D );

		Matrix<long> Al(6,5,0,1,1);
		Matrix<long> Bl(6,5,0,1,1);
		Matrix<long> Cl(5,6,0,1,1);
		Matrix<long> Dl(6,5,0,1,-1);

		EXPECT( true == (Al == Bl && Bl == Al) );
		EXPECT( true == (!(Al == Cl) && Al != Cl) );
		EXPECT( Bl != Dl );

		Matrix<float> Af(6, 5, 0, 1.0, 1.0);
		Matrix<float> Bf(6, 5, 0, 1.0, 1.0);
		Matrix<float> Cf(5, 6, 0, 1.0, 1.0);
		Matrix<float> Df(6, 5, 0, 1.0, -1.0);

		EXPECT( true == (Af == Bf && Bf == Af) );
		EXPECT( true == (!(Af == Cf) && Af != Cf) );
		EXPECT( Bf != Df );

		Matrix<double> Ad(6, 5, 0, 1.0, 1.0);
		Matrix<double> Bd(6, 5, 0, 1.0, 1.0);
		Matrix<double> Cd(5, 6, 0, 1.0, 1.0);
		Matrix<double> Dd(6, 5, 0, 1.0, -1.0);

		EXPECT( true == (Ad == Bd && Bd == Ad) );
		EXPECT( true == (!(Ad == Cd) && Ad != Cd) );
		EXPECT( Bd != Dd );

		Matrix<long double> Ald(6, 5, 0, 1.0, 1.0);
		Matrix<long double> Bld(6, 5, 0, 1.0, 1.0);
		Matrix<long double> Cld(5, 6, 0, 1.0, 1.0);
		Matrix<long double> Dld(6, 5, 0, 1.0, -1.0);

		EXPECT( true == (Ald == Bld && Bld == Ald) );
		EXPECT( true == (!(Ald == Cld) && Ald != Cld) );
		EXPECT( Bld != Dld );
	},

	CASE( "Matrices addition and subtraction")
	{
		Matrix<float> A(6, 10, 0,1.0, 1.0);
		Matrix<float> B(6, 10, 0,1.0,2.0);
		Matrix<float> AplusB(6, 10, 0,1.0,3.0);
		Matrix<float> AminusB(6, 10, 0,1.0,-1.0);
		Matrix<float> BminusA(6, 10, 0,1.0,1.0);

		EXPECT( true == (AplusB == A + B && AplusB == B + A) );
		EXPECT( true == (AminusB == A - B && AminusB != B - A && BminusA == B - A) );
	},

	CASE( "Multiplication between a scalar and a matrix" )
	{
		float PI = 3.14;
		Matrix<float> A(4,3,0,1.0,1.0);
		Matrix<float> AtimesPI(4,3,0,1.0,PI);

		EXPECT( true == (AtimesPI == (A * PI) && AtimesPI == (PI * A)) );

		A *= PI;
		EXPECT( AtimesPI == A);
	},

	CASE( "Division of a matrix by a scalar" )
	{
		float PI = 3.14;
		Matrix<float> A(4, 3, 0, 1.0, PI);
		Matrix<float> AbyPI(4, 3, 0, 1.0, 1.0);
		EXPECT( true == (AbyPI == (A / PI)) );

		A /= PI;
		EXPECT( AbyPI == A);
	},

	CASE( "Multiplication of a matrix by a matrix" )
	{
		/*
		  2 0 0   3 3   6 6
		  0 2 0 * 3 3 = 6 6
		  0 0 2   3 3   6 6
		 */
		Matrix<float> A = Matrix<float>::identity(3, 3, 0, 2.0);
		Matrix<float> B(3, 2, 0, 1.0, 3.0);

		Matrix<float> AtimesB( 3, 2, 0, 1.0, 6.0 );

		EXPECT( (A * B) == AtimesB );

		EXPECT_THROWS_AS( B * A, std::domain_error );

		/*
		  1 2 * 4 3 = 8 5
		  3 4   2 1   20 13

		  4 3 * 1 2 = 13 20
		  2 1   3 4   5  8
		 */
		int _C[4] = {1,2,3,4};
		int _D[4] = {4,3,2,1};

		Matrix<int> C(2,2,0,1,_C);
		Matrix<int> D(2,2,0,1,_D);

		EXPECT( (C * D) != (D * C) );
	},

	CASE( "Matrix transposition" ){
		/*
		  1 2 3    1 4
		  4 5 6 to 2 5
		           3 6
		 */
		int val[6] = {1,2,3,4,5,6};
		int val_t[6] = {1,4,2,5,3,6};
		Matrix<int> A( 2 ,3, 0, 1, val);
		Matrix<int> B( 3, 2, 0, 1, val_t);

		EXPECT( A.transpose() == B );
		EXPECT( B.transpose().transpose() == B );

	},

	CASE("Swapping rows"){
		int val[3] = {1,2,3};
		int swap[3] = {3,2,1};
		Matrix<int> A( 3 ,1, 0, 1, val);
		Matrix<int> B( 3 ,1, 0, 1, swap);

		EXPECT( A.swapRows(1,3) == B );
		EXPECT( B.swapRows(1,2) == B.swapRows(2,1) );
		EXPECT( A.swapRows(2,2) == A );

		EXPECT_THROWS_AS( B.swapRows(2,4), std::out_of_range );
		EXPECT_THROWS_AS( B.swapRows(0,2), std::out_of_range );
	},

	CASE("Swapping columns"){
		int val[6] = {1, 2, 3, 11, 22, 33};
		int swap[6] = {3, 2, 1, 33 , 22, 11};
		Matrix<int> A( 2 ,3, 0, 1, val);
		Matrix<int> B( 2 ,3, 0, 1, swap);

		EXPECT( A.swapColumns(1,3) == B );
		EXPECT( B.swapColumns(1,2) == B.swapColumns(2,1) );
		EXPECT( A.swapColumns(2,2) == A );

		EXPECT_THROWS_AS( B.swapColumns(2,4), std::out_of_range );
		EXPECT_THROWS_AS( B.swapColumns(0,2), std::out_of_range );
	},

	CASE("Concatenate matrices"){
		int val[4] = {1, 2, 1, 2};
		int val2[6] = {3, 4, 5, 3, 4, 5};
		int concat[10] = {1, 2, 3, 4, 5, 1, 2, 3, 4, 5};
		Matrix<int> A( 2 ,2, 0, 1, val);
		Matrix<int> B( 2 ,3, 0, 1, val2);
		Matrix<int> A_B( 2 ,5, 0, 1, concat);

		EXPECT( A.concat(B) == A_B );
		EXPECT_THROWS_AS( A_B.transpose().concat(A_B), std::domain_error );
	},

	CASE("Split a matrix in two"){
		int whole[4] = {1, 2, 3, 4};
		int left[2] = {1, 3};
		int right[2] = {2, 4};
		Matrix<int> A_B(2, 2, 0, 1, whole);
		Matrix<int> A(2, 1, 0, 1, left);
		Matrix<int> B(2, 1, 0, 1, right);

		Matrix<int> lMatrix;
		Matrix<int> rMatrix;

		A_B.split(1, lMatrix, rMatrix);
		EXPECT( lMatrix == A );
		EXPECT( rMatrix == B );
		EXPECT( lMatrix.concat(rMatrix) == A_B );
	},

	CASE("LU decomposition"){
		Matrix<float> L;
		Matrix<float> U;
		bool isSingular;

		float valA[9] = {1,4,-3,-2,8,5,3,4,7};
		float l[9] = {1,  0,  0,  -2,  1,  0, 3,  -0.5,  1 };
		float u[9] = {1,  4,  -3,  0,  16,  -1, 0,  0,  15.5};
		Matrix<float> A(3, 3, 0, 1, valA);
		Matrix<float> expectedL(3,3,0,1, l);
		Matrix<float> expectedU(3,3,0,1, u);

		isSingular = A.toLU(L, U);

		EXPECT( isSingular );
		EXPECT( L == expectedL );
		EXPECT( U == expectedU );
		EXPECT( ( L * U) == A );

		//This one requires a row exchange
		float valB[9] = {0,1,4, 1,0,3, 2,4,-7 };
		Matrix<float> B(3, 3, 0, 1, valB);

		isSingular = B.toLU(L, U);
		EXPECT( isSingular );
		EXPECT( ( L * U) == B );

		//This one is not singular
		float valC[9] = {1,2,3, 2,5,1, 1,3,-2 };
		Matrix<float> C(3, 3, 0, 1, valC);

		isSingular = C.toLU(L, U);

		EXPECT( !isSingular );
		EXPECT( ( L * U) == C );
	}
};

int main( int argc, char * argv[] )
{
    return lest::run( specification, argc, argv );
}




