#include "dcl.h"

void NextLine(char *line_buf, FILE* fp);


// Coefficients, obtained by matlab FDAtool; 50-tap FIR, passband 2500Hz, stopband 3000Hz
int H[51] =  {-35,    -219,   -503,   -494,   -8,     347,    19,     -348,   26,
        404,    -103,   -485,   220,    579,    -398,   -678,   663,    774,
        -1073,  -859,   1769,   925,    -3279,  -968,   10363,  17367,  10363,
        -968,   -3279,  925,    1769,   -859,   -1073,  774,    663,    -678,
        -398,   579,    220,    -485,   -103,   404,    26,     -348,   19,
        347,    -8,     -494,   -503,   -219,   -35};



/**********************************Macro Definition****************************/
#define PI 3.1415926535

/*****************************Data Type Definition****************************/
typedef int DataType;

void FIR( int Freq )
{
	DataType Xin[FIR_Input_Size], Output=0, sum=0;
	int sig=0, i=0, j=0, nNow=0, no=0;

	FILE* fp = NULL;

	const int ORDER = 50;

//	printf("Input Frequents: %d\n", Freq6);

	fp = fopen("FIR_data.dat", "w");

	fprintf(fp, "*** Number of input data ***\n");
	fprintf(fp, "*%d\n\n", FIR_Input_Size);

//	printf("Inputs: \n");
	for(i = 0; i < FIR_Input_Size; i++) // simulated input
	{
		// a combination of a 2000Hz signal and a 4000Hz signal
		//Xin[i] = ((sin(PI*i*2000/10000/8) + sin(PI*i*4000/10000/8)));

		// a combination of a 2700Hz signal and a 2900Hz signal
		//Xin[i] = ((sin(2*PI*i*2700/10000) + sin(2*PI*i*2900/10000)));

		
		//Xin[i] = sin(2*PI*i*Freq1/40000) + sin(2*PI*i*Freq4/40000);
		Xin[i] = ( (float)sin(2*PI*i* Freq /10000) * 1000 + 1000 );

		//printf("%d\n", Xin[i]);

		fprintf(fp, "%d\n", Xin[i]);
	}
	fprintf(fp, "-1\n" );

	for( i = 0; i < FIR_Output_Size; i++ )
	{
		//input new x[]
		sum = 0;
		for (j=0; j<ORDER; j++)//FIR computing
		{
			sum = sum + H[ORDER - 1 - j] * Xin[i+j];
			//printf("%d (H[%d]) * %d (Xin[%d]) --> %d \n",  H[ORDER-1-j], ORDER-1-j, Xin[i+j], i+j, H[ORDER - 1 - j] * Xin[i+j]);
		}
		Output = sum / 32767 ;
		accurOuts[i+1] = Output;
		
		//Output
	//	printf("%d\n", Output);
	}
	printf("\n\n");

	fflush(fp);
	fclose(fp);
    
}



/* Primary Inputs are modeled as being stored in memory:

1. If the address of an operand (addr1/addr2) is from [1, node_num], it is from an internal output of an operator

2. If the address is from [ node_num + 1, node_num + 1 + 49 ], it is from H[]

3. If the address is from [ (node_num + 1) * 2, +infinity ), it is from Xin[]

*/


void Load_Set_Primary_Inputs_FIR( char* fname )
{
	char infile[64];
        char line_buf[128];
        FILE *fp = NULL;
        int dat;
        int i, n;
	int addr1, addr2;

        sprintf( infile, "%s_data.dat", fname );
        if( (fp = fopen(infile,"r")) == NULL ) {
                printf( "Cannot open file %s\n", infile );
                exit(0);
        }


        /* both Xin and H[] start from index 0 */
        /* stop signal is -1 */
        i = 0;
        NextLine( line_buf, fp );
        sscanf( line_buf, "%d", &dat );
        while( dat >= 0 ) {
                Xin[i] = dat;
                i++;

                NextLine( line_buf, fp );
                sscanf( line_buf, "%d", &dat );
        }

	if( node_num == 0 ) {
		printf("Node number is 0. Error\n");
		exit(0);
	}

	/* Set primary inputs for FIR */

	InternalAddrMax = node_num;
        HAddrBase = node_num + 1;
        XinAddrBase = ( node_num + 1 ) * 2;


	for( n = 1; n <= node_num; n++ ) {
		addr1 = nodes[n].addr1;
		addr2 = nodes[n].addr2;

		/* initialize left and right operands for all nodes */
                nodes[n].OprdL = nodes[n].OprdR = -1;
                /* output value is still not valid for each node */
                nodes[n].outValid = 0;

                /* if both inputs are primary inputs, the node is ready to be computed*/
                if( addr1 > InternalAddrMax && addr2 > InternalAddrMax ) {
                        nodes[n].outValid = 3;  // binary b'11
                }

                /* get the left and right operands for a node */
                /* if addr1 is internal, it is still not ready at this stage */
                /* if addr1 is between [node_num+1, node_num+1+49], it is from H[] */
                if( addr1 >= HAddrBase && addr1 < XinAddrBase ) {
                        nodes[n].OprdL = H[ addr1 - HAddrBase ];
			nodes[n].outValid |= 1;  // left is valid now
                //      printf("left oprd of node %d: %d\n", n, nodes[n].OprdL);
                }
                /* if addr1 is between [(node_num+1]) * 2, +infinity), it is from Xin[] */
                else if( addr1 >= XinAddrBase ) {
                        nodes[n].OprdL = Xin[ addr1 - XinAddrBase ];
			nodes[n].outValid |= 1;
                //      printf("left oprd of node %d: %d\n", n, nodes[n].OprdL);
                }

                if( addr2 >= HAddrBase && addr2 < XinAddrBase ) {
                        nodes[n].OprdR = H[ addr2 - HAddrBase ];
			nodes[n].outValid |= 2;
                //      printf("right oprd of node %d: %d\n", n, nodes[n].OprdR);
                }
                else if( addr2 >= XinAddrBase ) {
                        nodes[n].OprdR = Xin[ addr2 - XinAddrBase ];
			nodes[n].outValid |= 2;
                //      printf("right oprd of node %d: %d\n", n, nodes[n].OprdR);
                }
	}
}

