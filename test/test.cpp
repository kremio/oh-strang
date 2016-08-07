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
    	IntMatrix<2,2> matrix;
    	for( auto matrixValuesIterator = matrix.begin(); matrixValuesIterator != matrix.end(); matrixValuesIterator++)
    	{
    		EXPECT( 0 == *matrixValuesIterator);
    	}
    },

    CASE( "Constructor accepting a single value fills matrix with the value" )
    {
    	int myValue = -10;
    	IntMatrix<3,2> matrix(myValue);
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
    	IntMatrix<3,2> matrix(myValues);
    	auto it = myValues.begin();
    	for( auto matrixValuesIterator = matrix.begin(); matrixValuesIterator != matrix.end() && it != myValues.end(); matrixValuesIterator++)
    	{
    		EXPECT( *it++ == *matrixValuesIterator);
    	}
    },

	CASE( "The identity() static function creates a matrix where each value equals zero except for the values on its diagonal" )
	{
    	auto Identity4by3 = IntMatrix< 4, 3>::identity();
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
		IntMatrix<6, 5> A(1);
		IntMatrix<6, 5> B(1);
		IntMatrix<5, 6> C(1);
		IntMatrix<6, 5> D(-1);

		EXPECT( true == (A == B && B == A) );
		EXPECT( true == (!(A == C) && A != C) );
		EXPECT( B != D );

		IntegerTypeMatrix<long, 6, 5> Al(1);
		IntegerTypeMatrix<long, 6, 5> Bl(1);
		IntegerTypeMatrix<long, 5, 6> Cl(1);
		IntegerTypeMatrix<long, 6, 5> Dl(-1);

		EXPECT( true == (Al == Bl && Bl == Al) );
		EXPECT( true == (!(Al == Cl) && Al != Cl) );
		EXPECT( Bl != Dl );

		FloatMatrix<6, 5> Af(1.0);
		FloatMatrix<6, 5> Bf(1.0);
		FloatMatrix<5, 6> Cf(1.0);
		FloatMatrix<6, 5> Df(-1.0);

		EXPECT( true == (Af == Bf && Bf == Af) );
		EXPECT( true == (!(Af == Cf) && Af != Cf) );
		EXPECT( Bf != Df );

		DoubleMatrix<6, 5> Ad(1.0);
		DoubleMatrix<6, 5> Bd(1.0);
		DoubleMatrix<5, 6> Cd(1.0);
		DoubleMatrix<6, 5> Dd(-1.0);

		EXPECT( true == (Ad == Bd && Bd == Ad) );
		EXPECT( true == (!(Ad == Cd) && Ad != Cd) );
		EXPECT( Bd != Dd );

		LongDoubleMatrix<6, 5> Ald(1.0);
		LongDoubleMatrix<6, 5> Bld(1.0);
		LongDoubleMatrix<5, 6> Cld(1.0);
		LongDoubleMatrix<6, 5> Dld(-1.0);

		EXPECT( true == (Ald == Bld && Bld == Ald) );
		EXPECT( true == (!(Ald == Cld) && Ald != Cld) );
		EXPECT( Bld != Dld );
	},

	CASE( "Matrices addition and subtraction")
	{
		FloatMatrix<6, 10> A(1.0);
		FloatMatrix<6, 10> B(2.0);
		FloatMatrix<6, 10> AplusB(3.0);
		FloatMatrix<6, 10> AminusB(-1.0);
		FloatMatrix<6, 10> BminusA(1.0);

		EXPECT( true == (AplusB == A + B && AplusB == B + A) );
		EXPECT( true == (AminusB == A - B && AminusB != B - A && BminusA == B - A) );
	},

	CASE( "Multiplication between a scalar and a matrix")
	{
		float PI = 3.14;
		FloatMatrix<4, 3> A(1.0);
		FloatMatrix<4, 3> AtimesPI(PI);

		std::cout << AtimesPI.toString() << "\n";
		std::cout << (A * PI).toString() << "\n";
		std::cout << (PI * A).toString() << "\n";

		EXPECT( true == (AtimesPI == (A * PI) && AtimesPI == (PI * A)) );

		A *= PI;
		EXPECT( AtimesPI == A);
	},

	CASE( "Division of a matrix by a scalar")
	{
		float PI = 3.14;
		FloatMatrix<4, 3> A(PI);
		FloatMatrix<4, 3> AbyPI(1.0);
		std::cout << (A / PI).toString() << "\n";
		std::cout << AbyPI.toString() << "\n";
		EXPECT( true == (AbyPI == (A / PI)) );

		A /= PI;
		EXPECT( AbyPI == A);
	},

};

int main( int argc, char * argv[] )
{
    return lest::run( specification, argc, argv );
}




