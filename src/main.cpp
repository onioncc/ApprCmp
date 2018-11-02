#define MAIN

#include "dcl.h"


void ReadInput( char* name );
void InitGraph();
void Sched_Bind( int iter );
void saveSolution();
void printSchedBind_final();

void InitResource()
{

	FU_num = 3;

	FUs[1].init(1, 2000, 1, 1000);	// ADD1 -> op: 1
	FUs[2].init(2, 5000, 1, 4000);	// MUL1 -> op: 2
	FUs[3].init(3, 3000, 1, 4000);	// MUL2 -> op: 3

}

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

	InitResource();
	ReadInput( name );
	InitGraph();	
	printf("DFG Initialization Finished\n");

	printf("##### Scheduling and Binding Started #####\n");
	energy_global = -1;
	for(int i = 1; i <= 10; i++) {
		Sched_Bind( i );
		if( energy_global == -1 || energy_global > energy_local ) {
			
			// accept as best global solution
			printf("saveSolution\n");
			saveSolution();
		}
	}
	printf("\n\n##### Scheduling and Binding Finished #####\n");
	printf("##### Final Results #####\n\n");
	printSchedBind_final();

	return 0;
}
