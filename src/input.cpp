

#include "dcl.h"
#include "utility.h"


void NextLine(char *line_buf, FILE* fp)
{
  char *seek;
  while(fgets(line_buf,128,fp) != NULL){
         for(seek = line_buf; *seek != '\0'; seek++ ){
           if(!isspace(*seek))break;
         }
     if(*seek !='\0' && line_buf[0] != '*') break;
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


void ReadInput( char* name )
{

	FILE* fo;

	char inFileName[128];
	sprintf(inFileName, "%s.dat", name); 
	char line_buf[128];
	int n1, n2, nb, op;

	if( (fo = fopen(inFileName, "r")) == NULL ) {
		printf("Cannot open input file %s\n", inFileName);
		exit(0);
	}


	nd_max = 0;
	ed_max = 0;


	// Initialize node array and edge array
	for( nb = 1; nb < ND; nb++ ) {
		node[nb].in_degree = 0;
		node[nb].out_degree = 0;
	}


	// Input Node Data
	NextLine(line_buf, fo);
    sscanf(line_buf,"%d %d", &nb, &op);
    while(nb > 0) {
    	maximize(nd_max, nb);
   		node[nb].op = op;

		NextLine(line_buf, fo);
		sscanf(line_buf,"%d %d",&nb, &op);

    }
    printf("nd_max: %d\n", nd_max);

    // Input Edge Data
    NextLine(line_buf, fo);
    sscanf(line_buf, "%d %d", &n1, &n2);
    while(n1 > 0) {
    	ed_max++;

    	// Edge:  n1 ---> n2
    	edge[ed_max].node = n1;
    	edge[ed_max].id = ed_max;

    	edge[(mate(ed_max))].node = n2;
    	edge[(mate(ed_max))].id = ed_max;


		NextLine(line_buf, fo);
    	sscanf(line_buf, "%d %d", &n1, &n2);
    }
    printf("ed_max: %d\n", ed_max);

}



void addEdge( int e, int n1, int n2 )
{
	edge[e].next = node[n1].edge;
	node[n1].edge = e;

	edge[mate(e)].next = node[n2].edge;
	node[n2].edge = mate(e);

	// update node degree
	node[n1].out_degree++;
	node[n2].in_degree++;
}


void OutputGraph()
{
	printf("All Edges\n");
	for( int e = 1; e <= ed_max; e++ ) {
		printf("%d->%d  ", edge[e].node, edge[mate(e)].node);
	}
	printf("\n");


	for(int n = 1; n <= nd_max; n++) {
		printf("Edges from node %d\n", n);
		for(int e = node[n].edge; e > 0; e = edge[e].next) {
			if( e > ed_max ) continue;

			int n1 = edge[e].node;
			int n2 = edge[mate(e)].node;
			printf("%d->%d ", n1, n2);
		}
		printf("\n");
		printf("In degree: %d, Out degree: %d\n\n", node[n].in_degree, node[n].out_degree);
	}


}


void InitGraph()
{
	int n, n1, n2, e;

	// Initialize graph data
	for( e = 1; e < ED1; e++ ) {
		edge[e].next = 0;
	}
	for( n = 1; n < ND; n++ ) {
		node[n].edge = 0;
	}


	for( e = 1; e <= ed_max; e++ ) {
		n1 = edge[e].node;
		n2 = edge[mate(e)].node;

		addEdge(e, n1, n2);
	}

	//OutputGraph();
}



void InitResource()
{

	FU_num = 3;

	FUs[1].init(1, 2000, 1, 1000);	// ADD1 -> op: 1
	FUs[2].init(2, 5000, 1, 4000);	// MUL1 -> op: 2
	FUs[3].init(3, 3000, 1, 4000);	// MUL2 -> op: 3

}