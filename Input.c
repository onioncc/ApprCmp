#include <stdio.h>
#include "dcl.h"


extern int H;	// from FIR.c

void NextLine(char *line_buf, FILE* fp)
{
	char *seek;
	while( fgets(line_buf, 128, fp) != NULL ) {
		for( seek = line_buf; *seek != '\0'; seek++ ) {
			if(!isspace(*seek)) break;
		}
		if( *seek !='\0' && line_buf[0] != '*' ) break;
	}
}

int Strcmp(char* s1, char* s2)
{
	while (*s1 && *s2){
		if (*s1++ != *s2++)
			return 0;
	}
	return *s1 == *s2;
}

/**************************************************/

void InputDFG(char *fname)
{
	char infile[64];
	char line_buf[128];
	FILE *fp1 = NULL, *fp2 = NULL;
	int n, op, in1, in2, pr, n1, n2;

	/* fp1 : the node file */
	sprintf( infile, "%s_node.dat", fname );
	if( (fp1 = fopen(infile,"r")) == NULL ) {
		printf( "Cannot open file %s\n", infile );
		exit(0);
	}

	/* fp2 : the edge file */
	sprintf( infile, "%s_edge.dat", fname );
	if( (fp2 = fopen(infile,"r")) == NULL ) {
		printf( "Cannot open file %s\n", infile );
		exit(0);
	}

	/* read node info */
	/*NextLine( line_buf, fp1 );
	sscanf( line_buf, "%d", &node_num );*/

	node_num = 0;
	NextLine( line_buf, fp1 );
	sscanf( line_buf, "%d", &n );
	while( n > 0 ) {
		node_num++;
		nodes[node_num].id = n;
		nodes[node_num].edge = 0;

		/* operation type */
		NextLine( line_buf, fp1 );
		sscanf( line_buf, "%d", &op );
		nodes[node_num].op = op;

		/* inputs: addr1 and addr2 */
		NextLine( line_buf, fp1 );
		sscanf( line_buf, "%d %d", &in1, &in2 );
		nodes[node_num].addr1 = in1;
		nodes[node_num].addr2 = in2;

		/* is primary output */
		NextLine( line_buf, fp1 );
		sscanf( line_buf, "%d", &pr );
		nodes[node_num].primOut = pr;
		
		NextLine( line_buf, fp1 );
		sscanf( line_buf, "%d", &n );
	}
	printf( "Node Info have been read\n" );

	/* read edge info */
	edge_num = 0;
	NextLine( line_buf, fp2 );
	sscanf( line_buf, "%d %d", &n1, &n2 );
	while( n1 > 0 ) {
		edge_num++;
		edges[edge_num].id = edge_num;
		edges[edge_num].node = n1;
		edges[edge_num].next = 0;

		edges[mate(edge_num)].id = edge_num;
		edges[mate(edge_num)].node = n2;
		edges[mate(edge_num)].next = 0;

		NextLine( line_buf, fp2 );
                sscanf( line_buf, "%d %d", &n1, &n2 );
	}
	printf( "Edge Info have been read\n" );

	fclose( fp1 );
	fclose( fp2 );

}

void InitialFUTypes()
{
	int n;
	for( n = 1; n <= node_num; n++ ) {
		if( nodes[n].op == 1 ) {	// ADD
			nodes[n].FU_id = 1;	// LOA_add
		}
		else if( nodes[n].op == 2 ) {	// MUL
			nodes[n].FU_id = 2;	// UDM_MUL
		}
	}
}






