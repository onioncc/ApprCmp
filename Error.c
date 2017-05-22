#include "dcl.h"


float outErrors[DCL_OUT];	// to store relative errors on outputs
float outErrors_avg[DCL_OUT];	// average relative error on each output of all input patterns
float outErrors_max[DCL_OUT];	// maximum relative error on each output among all input patterns


void ErrorInitialization()
{
	int i;

	if( errConstr == 0 )
		errConstr = 50.0;	// default error constraint;
	
	for( i = 1; i <= DCL_OUT; i++ )
		outErrors[i] = outErrors_avg[i] = outErrors_max[i] = 0;
}


int ErrorCheck()
{
	int i;
	float err;	// relative error
	int pass;

	pass = 1;

	/* process */

	for( i = 1; i <= outputNum; i++ ) {
		if( tbPtr == 1 ) {
			err = ((float)( (int)(primOuts[i] / 32767) - accurOuts[i] )) / accurOuts[i] * 100.0;
			printf("Output %d:\t Prim %d, \t Accur %d, \t RelErr %%%.2f\n", i, primOuts[i] / 32767, accurOuts[i], err);
			outErrors[i] = err;
		}
		
		if( err >= errConstr )
			pass = 0;
	}

	/*printf("Output errors: ");
	for( i = 1; i <= outputNum; i++ ) {
		printf("\t %4.2f", outErrors[i]);
	}
	printf("\n");*/

	return pass;
}


