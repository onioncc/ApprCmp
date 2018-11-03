#include "dcl.h"
#include "classes.h"


void ReadPrimInputs( char* name, int set );
void NextLine(char *line_buf, FILE* fp);

#define PRIM_IN_DCL ND * 2
float prim_in_vals[PRIM_IN_DCL];


/*void ReadPrimInputs( char* name )
{
	char inFileName[128];
	sprintf(inFileName, "./testcase/%s_in.dat", name); 

	char line_buf[128];
	float prim_in;
	FILE* fo;

	if( (fo = fopen(inFileName, "r")) == NULL ) {
		printf("Cannot open input file %s\n", inFileName);
		exit(0);
	}


	// Read primary inputs value
	for( int i = 1; i <= prim_in_num; i++ ) {
		NextLine(line_buf, fo);
    	sscanf(line_buf,"%f", &prim_in);	
    	prim_in_vals[i] = prim_in;
	}

    printf("# of primary inputs: %d\n", prim_in_num);
    for(int i = 1; i <= prim_in_num; i++) {
    	printf("%f ", prim_in_vals[i]);
    }
    printf("\n");
}*/



void ReadPrimInputs( char* name, int set )
{
	char inFileName[128];
	sprintf(inFileName, "./testcase/%s.bin", name); 
	float prim_in;
	
	ifstream ifs(inFileName, ios::in | ios::binary);
	ifs.seekg( (set-1) * 4 * prim_in_num);
	ifs.read((char*)(&prim_in_vals[1]), prim_in_num*sizeof(float));

    printf("# of primary inputs: %d\n", prim_in_num);
    for(int i = 1; i <= prim_in_num; i++) {
    	printf("%f ", prim_in_vals[i]);
    }
    printf("\n");
}


void AssignDFGPrimIn()
{
	int nd;
	for( nd = 1; nd < nd_max; nd++ ) {
		printf("Node %d\n", nd);

		if( node[nd].prim_in1 > 0 ) {
			node[nd].prim_in1_val = prim_in_vals[node[nd].prim_in1];
			printf("Prim In1: %f\n", node[nd].prim_in1_val);
		}
		if( node[nd].prim_in2 > 0 ) {
			node[nd].prim_in2_val = prim_in_vals[node[nd].prim_in2];
			printf("Prim In2: %f\n", node[nd].prim_in2_val);
		}
	}

}

int checkDFGPrimIn()
{
	int nd, valid;

	valid = 1;
	for( nd = 1; nd < nd_max; nd++ ) {
		if( node[nd].in_degree == 2 || node[nd].op == 3 ) {
			continue;
		}
		if( node[nd].in_degree == 1 ) {
			if( node[nd].prim_in1 == 0 ) {
				printf("Not enough primary input for node %d\n", nd);
				valid = 0;
			}
		}
		if( node[nd].in_degree == 0 ) {
			if( node[nd].prim_in1 == 0 || node[nd].prim_in2 == 0 ) {
				printf("Not enough primary input for node %d\n", nd);
				valid = 0;
			}
		}
	}
	return valid;
}



void ComputeAlongDFG()
{
	Queue Q;

	Q.init();

	int nd, e;
	int e1, e2;
	float oprd1, oprd2, res;

	for(nd = 1; nd <= nd_max; nd++) {
		// initialize temporary in/out degree
		node[nd].in_dgr_tmp = node[nd].in_degree;
		node[nd].out_dgr_tmp = node[nd].out_degree;
	
		if( node[nd].in_dgr_tmp == 0 ) {
			Q.insertElement(nd);
		}
	}

	while( !Q.isEmpty() ) {
		nd = Q.getElement();

		// compute its output
		e1 = 0; e2 = 0;
		
		for(e = node[nd].edge; e > 0; e = edge[e].next) {
			if( e <= ed_max ) continue;
			if( e1 == 0 ) e1 = e;
			else e2 = e;
		}

		if( e1 == 0 ) {
			oprd1 = node[nd].prim_in1_val;
			oprd2 = node[nd].prim_in2_val;
		}
		else if( e2 == 0 ) {
			oprd1 = edge[e1].DFG_value;
			oprd2 = node[nd].prim_in1_val;
		}
		else {
			oprd1 = edge[e1].DFG_value;
			oprd2 = edge[e2].DFG_value;
		}
		
		
		switch( node[nd].op ) {
			case 1: res = oprd1 + oprd2;
					break;
			case 2: res = oprd1 * oprd2;
					break;
			case 3: res = sqrt(oprd1);
					break;
			default:
					res = 0;
					break;
			// ... more approximate types
		}

		printf("Output of Node %d: %f\n", nd, res);

		// if node is already the primary output
		if( node[nd].prim_out > 0 ) {
			node[nd].prim_out_val = res;
		}
		else {
			// propagate the result along its output edges
			for(e = node[nd].edge; e > 0; e = edge[e].next) {
				if( e > ed_max ) continue;

				edge[e].DFG_value = res;
				edge[mate(e)].DFG_value = res;

				// decrease the degrees of all its successor nodes by one
				int n_s = edge[mate(e)].node;
				assert( node[n_s].in_dgr_tmp >= 1);

				node[n_s].in_dgr_tmp--;
				if( node[n_s].in_dgr_tmp == 0 ) {
					Q.insertElement(n_s);
				}
			}
		}
	}
}



void printPrimOuts()
{
	for(int nd = 1; nd <= nd_max; nd++) {
		if( node[nd].prim_out > 0 ) {
			printf("PO %d: %f\n", node[nd].prim_out, node[nd].prim_out_val);
		}
	}
}


void DFGSimulation( char* name )
{
	printf("Conducting DFG Simulation\n");

	// primary input set 1
	ReadPrimInputs( name, 1 );
	AssignDFGPrimIn();
	if( checkDFGPrimIn() == 0) {
		printf("Primary inputs are not valid. Please check %s.dat again.\n", name);
	}

	ComputeAlongDFG();
	printPrimOuts();

	// primary input set 2
	ReadPrimInputs( name, 2 );
	AssignDFGPrimIn();
	ComputeAlongDFG();
	printPrimOuts();

	// primary input set 3
	ReadPrimInputs( name, 3 );
	AssignDFGPrimIn();
	ComputeAlongDFG();
	printPrimOuts();
}