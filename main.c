#define MAIN
#include "dcl.h"

#define DEBUG

#ifndef DEBUG
// the number of approximate solutions
#define		APPR_SOL	100
// the number of input patterns
#define		INPUT_PAT	10000
#else
#define		APPR_SOL	5
#define		INPUT_PAT	10
#endif


void InputDFG(char *caseName);
void GraphTransform();
int Strcmp(char* s1, char* s2);
void InitialFUTypes();


unsigned long long timeGetTime()
{
        struct timeval tv;
        gettimeofday( &tv, NULL );

        return tv.tv_sec*1000000 + tv.tv_usec;
}

int main( int args, char** argv )
{
        char caseName[64];
        int i, j;
	int inp;	// input pattern
	int pass;

	void (*accurateFunc)(int);
	void (*loadPrimInputs)(char*);

        printf("\nProgram Started\n\n");

        if( args <= 1 ) {
                printf("input name of the testcase (test1, FIR, etc.) > ");
                scanf("%s", caseName);
        }
        else {
                strcpy( caseName, argv[1] );
                for( i = 2; i <= args - 1; i += 2) {
                        // parameters
                        if( Strcmp( argv[i], (char*)"-e" ) ) errConstr = atof( argv[i+1] );
                        else {
                                printf(" *** invalid command input %s\n", argv[i]);
                                exit(-1);
                        }
                }
        }


	printf("\n === Synthesis for testbench %s === \n\n", caseName);

	/* Read DFG of the testbench */
	InputDFG( caseName );
	GraphTransform();
	InitialFUTypes();

	/* to output a solution space */
	for( i = 1; i <= APPR_SOL ; i++ ) {

		/* perturbate an approximate solution */
		/* eg. the simplest, you may want to replace an accurate FU by an approximate one */
		/* or, you are allowed to propose any method to perform the perturbation */
		// GenerateApprmSolution();

		/* Apply list scheduling to determine the resource usage */
		// ListScheduling();

		/* Output the resource usage*/
		// OutputResourceUsage();


		for( j = 1; j <= INPUT_PAT; j++ ) {

			/* Stage 1: Generate primary inputs for DFG */ 
			// GeneratePrimInputs();

			/* Stage 2: Compute accurate solutions */
			// AccurateFunc();

			/* Stage 3: Compute approximate solutions */
			// DFGSimulate();

			/* Stage 4: Compare accurate and approximate solutions */
			// ComputePrimError();
		}

		/* Output the errors on primary outputs: average and maximum (temperily) */

	}
}
