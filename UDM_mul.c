#include "dcl.h"

void Decimal_To_Binary_K_bit( short int* sequence, int n, int K, int withSign );
int Binary_To_Decimal_K_bit( short int* n, int K, int withSign );
int UDM_mul_K_bit_R( int A, int B, int K );


/* basic truth table for inaccurate 2*2 multiplier */
short int UDM_mul_2bit( int A1A0, int B1B0 )
{
	/* the only case for inaccurate result */
	if( abs(A1A0) == 3 && abs(B1B0) == 3 ) {
		return 7;
	}
	else
		return abs(A1A0 * B1B0);
}


/* K bit refers to the bit number of oprands */
/* for multiplier, the bit nubmer of result is 2*K */
/* K is expected to be 2^n */

int UDM_mul_K_bit( int A, int B, int K )
{
	int result;

	result = UDM_mul_K_bit_R( abs(A), abs(B), K );

	/* for multiplication sign bits can be ignored at first */
	/* to be determined after all the computation */
	if( (A > 0 && B < 0) || (A < 0 && B > 0) ) {
		result = 0 - result;
	}
	return result;
}


int UDM_mul_K_bit_R( int A, int B, int K )
{
	int result;
	int AL, BL, AH, BH;
	int ALBL;
	int ALBH;
	int AHBL;
	int AHBH;
	short int A_binary[K];
	short int B_binary[K];
	short int AL_binary[K/2];
	short int BL_binary[K/2];
	short int AH_binary[K/2];
	short int BH_binary[K/2];

	int i;

	Decimal_To_Binary_K_bit( A_binary, A, K, 0 );
	Decimal_To_Binary_K_bit( B_binary, B, K, 0 );

	/* for 2-bit multiplier, return the result in the truth table */
	if( K == 2 ) {
		result = UDM_mul_2bit( A, B );
		return result;
	}	

	/* A_binary[0] is LSB */
	/* A_binary[0] - A_binary[K/2-1] : AL_binary[0] - AL_binary[K/2-1] */
	/* A_binary[K/2] - A_binary[K-1] : AH_binary[0] - AH_binary[K/2-1] */
	for( i = 0; i <= K/2-1; i++ ) {
		AL_binary[i] = A_binary[i];
		BL_binary[i] = B_binary[i];
	}

	for( i = K/2; i <= K-1; i++ ) {
		AH_binary[i-K/2] = A_binary[i];
		BH_binary[i-K/2] = B_binary[i];
	}

	AL = Binary_To_Decimal_K_bit( AL_binary, K/2, 0 );
	AH = Binary_To_Decimal_K_bit( AH_binary, K/2, 0 );
	BL = Binary_To_Decimal_K_bit( BL_binary, K/2, 0 );
	BH = Binary_To_Decimal_K_bit( BH_binary, K/2, 0 );
	

	ALBL = UDM_mul_K_bit_R( AL, BL, K/2 );
	ALBH = UDM_mul_K_bit_R( AL, BH, K/2 );
	AHBL = UDM_mul_K_bit_R( AH, BL, K/2 );
	AHBH = UDM_mul_K_bit_R( AH, BH, K/2 );

	result = ALBL + (AHBH << K) + (ALBH << (K/2)) + (AHBL << (K/2));

	return result;
	
}


