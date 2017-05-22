#include "dcl.h"


/* NOTE: the bit-number includes the two sign bits on the MSB side */
/* positive value: 00 + xxxxx */
/* negative value: 11 + xxxxx */
/* for a K-bit sequence, sequence[ K-1 ] and sequence [ K-2 ] are sign bits */

int Binary_To_Decimal_K_bit( short int* n, int K, int withSign );
void Complement_Binary( short int* sequence, int K );

void print_Binary_Sequence( short int* seq, int K, int withSign )
{
	int i, n;

	printf(" %d %d | ", seq[ K-2 ], seq[ K-1 ]);
	for( i = K - 1 - 2; i >= 0; i-- )
		printf(" %d ", seq[i] );

	n = Binary_To_Decimal_K_bit( seq, K, withSign );
	printf("\t %d\n", n);

}



/* transfer an integer into a K bit binary sequence */
/* sequence[0] is LSB */
/* withSign: whether sign bits (2 bits) are neccesarry */
void Decimal_To_Binary_K_bit( short int* sequence, int n, int K, int withSign )
{
        int i, r, np;	// np is possitive n
        np = ( n >= 0 ) ? n : 0-n;

        for( i = 0; i <= K - 1; i++ )
                sequence[ i ] =  0;

	i = 0;
        while( np > 0 ) {
                r = np % 2;
                np = np / 2;
                sequence[ i ] = r;
                i++;
        }

	/* set sign bits */
	/* no need for positive values or 0 */
	if( withSign && n < 0 ) {
		if( sequence[ K-1 ] == 1 || sequence[ K-2 ] == 1 ) {
			printf("Integer %d overflows. Need more bits than %d\n", n, K);
			assert(0);
		}
		sequence[ K-1 ] = sequence[ K-2 ] = 1;
	}
}



/* transfer a K bit binary sequence into integer */
int Binary_To_Decimal_K_bit( short int* n, int K, int withSign )
{
	int i;
	int result;

	result = 0;

	if( withSign == 1 ) {
		/* except for two sign bits*/
		for( i = 0; i <= K - 1 - 2; i++ ) {
			result += pow( 2, i ) * n[ i ];
		}
		
		if( n[ K-1 ] == 1 && n[ K-2 ] == 1 )
                result = 0 - result;
	}
	else {
		for( i = 0; i <= K - 1; i++ ) {
                        result += pow( 2, i ) * n[ i ];
                }
	}

	return result;
}


/* get the complement binary code for a binary sequence of K-bit (including two sign-bits) */
/* if sign bits are 00, binary sequence remains unchanged */
/* if sign bits are 11, complement sequence is obtained by reversing all the bits (except for sign bits) and plusing 1 */
void Complement_Binary( short int* sequence, int K )
{
	int i;
	int sum, carry, add;
	short int tmpSequence[128];	// tempory binary sequence

	/* no operation is needed when two sign bits are 00 */
	if( sequence[ K-1 ] == 0 && sequence[ K-2 ] == 0 )
		return;

	/* reverse all bits of sequence except for sign bits */
	if( sequence[ K-1 ] != 1 || sequence[ K-2 ] != 1 ) {
		printf("Sign bits %d %d are wrong.\n", sequence[ K-1 ], sequence[ K-2 ]);
		assert(0);
	}

	/* get reversed code except for sign bits */
	for( i = 0; i <= K - 1 - 2; i++ ) {
		tmpSequence[i] = ( sequence[i] == 1 ) ? 0 : 1;
	}
	/* assign sign bits as 1 */
	tmpSequence[ K-1 ] = tmpSequence[ K-2 ] = 1;

	i = 0;
	carry = 1;
	while( carry == 1 ) {
		add = tmpSequence[i] + carry;
		carry = ( add == 2 ) ? 1 : 0;
		sum = ( add == 1 ) ? 1 : 0;

		tmpSequence[i] = sum;
		i++;
	}

	/* check if sign bits are affected */
	if( tmpSequence[ K-1 ] != 1 || tmpSequence[ K-2 ] != 1 ) {
		printf("Sign bits are wrong\n");
		assert(0);
	}

	/* copy tmpSequence back to sequence */
	for( i = 0; i <= K - 1; i++ ) {
		sequence[i] = tmpSequence[i];
	}
}

