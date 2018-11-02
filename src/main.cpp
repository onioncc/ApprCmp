#define MAIN

#include "dcl.h"


void ReadInput( char* name );
void InitGraph();
void InitScheduling();
void InitResource();


int main(int argc, char** argv)
{

	printf("##### Program Started #####\n");

	char name[128];
	if( argc == 1 ) {
		printf("Input Testbench >> ");
		int ret = scanf("%s", name );
	}
	else {
		sprintf(name, "%s", argv[1]);
	}

	ReadInput( name );
	InitGraph();
	InitResource();

	printf("DFG Initialization Finished\n");


	InitScheduling();

	return 0;
}
