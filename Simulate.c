#include "dcl.h"

extern int H[];
extern double grandom(void);

int LOA_add_K_bit( int n1, int n2, int K );
int UDM_mul_K_bit( int A, int B, int K );
int ETM_mul_K_bit( int A, int B, int K );


/* a node stack */
int nodeStk[DCL_ND]; 
int ptr;
int isEmpty()
{
        return ( ptr == 0 ) ? 1 : 0;
}
void initNodeStk()
{
        ptr = 0;
        for( int i = 0; i <= node_num; i++ ) nodeStk[i] = 0;
}
void pushNode( int n )
{
        nodeStk[ptr++] = n;
}
int popNode()
{
        if( isEmpty() ) return 0;
        return nodeStk[--ptr];
}       


long int largest = -1;
long int tmp = 0;


void DFGSimulate()
{
	int n, e, n1;
	int val;
	int A, B;

	/* initialize node stack */
	initNodeStk();

	/* collect initial ready node list*/
	for( n = 1; n <= node_num; n++ ) {
		if( nodes[n].outValid == 3 )
			pushNode( n );
	}

	while( (n = popNode()) != 0 ) {

		//printf("Node %d out of stack\n", n);

		A = nodes[n].OprdL;
		B = nodes[n].OprdR;

		//printf("Node %d : A %d B %d\n", n, A, B);

		if( nodes[n].op == 1 ) {
			/* if n is an approximate FU, the '+' may not be accurate */
			//val = nodes[n].OprdL + nodes[n].OprdR;
			//val = LOA_add_K_bit( A, B, 32 );
			//printf("%8d + %8d --> %8d\n", A, B, val );

			//if( largest < abs(val) ) largest = abs(val);


			if( nodes[n].FU_id == 0 ) {		// accurate ADD
				val = nodes[n].OprdL + nodes[n].OprdR;
			}
			else if( nodes[n].FU_id == 1 ) {	// LOA_add of 32 bit
				//val = LOA_add_K_bit( A, B, 32 );
				val = nodes[n].OprdL + nodes[n].OprdR;
			}


		}
		else if( nodes[n].op == 2 ) {
			//if( A > 32767 || B > 32767 ) {
			//	printf("Input overflow: A %d  B %d\n", A, B);
			//	exit(0);
			//}
			//val = UDM_mul_K_bit( A, B, 16 );
			//printf("%8d * %8d --> %8d\n", A, B, val );

			//if( largest < abs(val) ) largest = abs(val);

			if( nodes[n].FU_id == 0 ) {		// accurate MUL
				val = nodes[n].OprdL * nodes[n].OprdR;
			}
			else if( nodes[n].FU_id == 2 ) {	
				val = UDM_mul_K_bit( A, B, 16 );	// UDM approximate
				//val = ETM_mul_K_bit( A, B, 32 );

				printf("FOR DEBUG\n");
				printf("op %d :  app= %d, exact= %d, ( %.3f)\n", n, val / 32767, nodes[n].OprdL * nodes[n].OprdR / 32767, (float)(val - nodes[n].OprdL * nodes[n].OprdR + 0.0 ) / (nodes[n].OprdL * nodes[n].OprdR + 0.0) * 100 );

			}


		}
		
		nodes[n].outValue = val;

		/* if node n is an primary output */
		if( nodes[n].primOut > 0 ) {
			//printf("Primary Out %d is %d\n", nodes[n].primOut, nodes[n].outValue );
			primOuts[ nodes[n].primOut ] = nodes[n].outValue;
			continue;
		}

		/* update all its descendant operators */
		for( e = nodes[n].edge; e > 0; e = edges[e].next ) {
			if( e >= DCL_ED ) continue;

			n1 = edges[ mate(e) ].node;
			if( nodes[n1].addr1 == n ) {
				nodes[n1].OprdL = val;
				nodes[n1].outValid |= 1;	// left is valid now 
			}
			else if( nodes[n1].addr2 == n ) {
				nodes[n1].OprdR = val;
				nodes[n1].outValid |= 2;
			}
		}
		/* if both are valid */
		if( nodes[n1].outValid == 3 ) {
			pushNode( n1 );
			//printf("Node %d in stack\n", n1);
		}

	}

	/* all primary outpus */
	/*for( n = 1; n <= outputNum; n++ ) {
		printf( "%d\n", primOuts[n] / 32767);
	}*/

	
	//printf("===========Largest value : %d\n", largest);

	/* Clear flags of all nodes */
	/* 	nodes[n].outValid flag */
	for( n = 1; n <= node_num; n++ ) {
		nodes[n].outValid = 0;
	}


}
