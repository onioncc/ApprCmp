#include <stdio.h>
#include <stdlib.h>
#include "dcl.h"
void Decimal_To_Binary_K_bit( short int* sequence, int n, int K, int withSign );
int Binary_To_Decimal_K_bit( short int* n, int K, int withSign );
int ETM_mul_K_bit( int A, int B, int K );
int ETM_mul_K_bit_R( int A, int B, int K );

#define N	28

/*int main()
{
     return ETM_mul_K_bit(15,15,4,3);
}*/

/* K bit refers to the bit number of oprands */
/* N bit refers to the bit number of multipliation part£¬ and (K-N) is the bit of pprocimate part */
/* for multiplier, the bit nubmer of result is 2*K-1 */
int ETM_mul_K_bit( int A, int B, int K )
{
	int result;

	result = ETM_mul_K_bit_R( abs(A), abs(B), K );
	/* for multiplication sign bits can be ignored at first */
	/* to be determined after all the computation */
	if( (A > 0 && B < 0) || (A < 0 && B > 0) ) {
		result = 0 - result;
	}
	return result;
}

int ETM_mul_K_bit_R( int A, int B, int K )
{
    int result;
    int AH, BH,RL;
	int AHBH;
	short int A_binary[K];
	short int B_binary[K];
	short int AL_binary[K-N];
	short int BL_binary[K-N];
	short int AH_binary[N];
	short int BH_binary[N];
	short int RL_binary[2*(K-N)];

	int i,p;

	Decimal_To_Binary_K_bit( A_binary, A, K, 0 );
	Decimal_To_Binary_K_bit( B_binary, B, K, 0 );

    for( i = 0; i <= (K-N-1); i++ ) {
		AL_binary[i] = A_binary[i];
		BL_binary[i] = B_binary[i];
	}

	for( i = K-N; i <= K-1; i++ ) {
		AH_binary[i-(K-N)] = A_binary[i];
		BH_binary[i-(K-N)] = B_binary[i];
	}

	AH = Binary_To_Decimal_K_bit( AH_binary, N, 0 );
	BH = Binary_To_Decimal_K_bit( BH_binary, N, 0 );

	/* if one of the inputs are too small, skip the approximate part */
	/* and directly compute the accurate solution */
	/* otherwise it will occur a huge error of more than 99% */
	/* this is implemented by a determine logic generating a CTRL signal */

	if( AH <= 1 || BH <= 1 )
		return A * B;

	AHBH=AH*BH;



	/*Approximate part*/
	for (i=0;i<(K-N);i++){
        if (!((A_binary[K-N-1-i]==0)&&(B_binary[K-N-1-i]==0)))
        {
            for (p=(2*(K-N)-i-1);p>=0;p--)
            {
             RL_binary[p]=1;
            }
            i=N;
        }
        else
            RL_binary[2*(K-N)-i-1]=0;
        if (i==(K-N-1)&&RL_binary[K-N]==0) {
            for (i=0;i<K-N;i++) {
                    RL_binary[i]=0;
            }
        }
    }

    RL=Binary_To_Decimal_K_bit( RL_binary, 2*(K-N), 0 );
    result= (AHBH << (2*(K-N)))+ RL;
    return result;
}
