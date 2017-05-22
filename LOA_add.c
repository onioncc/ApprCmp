#include "dcl.h"

/* 32 bit adder */
#define M	24


void Decimal_To_Binary_K_bit( short int* sequence, int n, int K, int withSign );
int Binary_To_Decimal_K_bit( short int* n, int K, int withSign );
void print_Binary_Sequence( short int* seq, int K );
void Complement_Binary( short int* sequence, int K );

/* parameter: MSB m-bit and LSB n-bit */
/* n = K - m */
int LOA_add_K_bit( int n1, int n2, int K )
{
	int N = K - M;

	short int n1B[K];
	short int n2B[K];
	short int result_binary[K];
	int result;

	int i;
	short int add, sum, carry;
	float relErr;

	// get binary sequences of n1 and n2
	Decimal_To_Binary_K_bit( n1B, n1, K, 1 );
	Decimal_To_Binary_K_bit( n2B, n2, K, 1 );

	//printf("Original code\n");
	//print_Binary_Sequence( n1B, K );
	//print_Binary_Sequence( n2B, K );


	/* get complement binary code for n1 and n2 */
	Complement_Binary( n1B, K );
	Complement_Binary( n2B, K );

	//printf("Complement code\n");
	//print_Binary_Sequence( n1B, K );
	//print_Binary_Sequence( n2B, K );
	

	// initialize result_binary
	for( i = 0; i <= K - 1; i++ )
		result_binary[ i ] = 0;
	

	/* LSB n-bit is computed using OR */
	for( i = 0; i < N; i++ ) {
		result_binary[ i ] = ( n1B[i] | n2B[i] ) & 0x0001;
	}

	/* MSB m-bit is computed*/
	carry = n1B[ N-1 ] + n2B[ N-1 ] == 2 ? 1 : 0;


	for( i = N; i <= K - 1; i++ ) {
		add = n1B[i] + n2B[i] + carry;
		if( add == 0 || add == 2 )
			sum = 0;
		else sum = 1;

		result_binary[ i ] = sum;
		
		if( add == 2 || add == 3 )
			carry = 1;
		else carry = 0;
	}

	/* if sign bits of result are 00, result is possitive */
	if( result_binary[ K-1 ] == 0 && result_binary[ K-2 ] == 0 ) {
		result = Binary_To_Decimal_K_bit( result_binary, K, 1 );
	}
	/* if sign bits of result are 11, result is negative and complement transfer is necessary */
	else if( result_binary[ K-1 ] == 1 && result_binary[ K-2 ] == 1 ) {
		Complement_Binary( result_binary, K );
		result = Binary_To_Decimal_K_bit( result_binary, K, 1 );
	}
	else {
		printf("Result overflew for addition %d + %d\n.", n1, n2);
		assert(0);
	}

	Decimal_To_Binary_K_bit( result_binary, result, K, 1 );
	//print_Binary_Sequence( result_binary, K );	

	return result;

}
