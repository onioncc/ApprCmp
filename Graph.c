#include "dcl.h"

void addEdge( int e, int n1, int n2 )
{
        edges[e].next = nodes[n1].edge;
        nodes[n1].edge = e;
        
        edges[mate(e)].next = nodes[n2].edge; 
        nodes[n2].edge = mate(e);

	/* n1 ---    e    ---> n2 */
	/* n2 --- mate(e) ---> n1 */
}               

void OutputGraph()
{
	int n, e;
	for( n = 1; n <= node_num; n++ ) {
		printf("Node %d, edges:\n", n);
		for( e = nodes[n].edge; e > 0; e = edges[e].next ) {
			/* if e > DCL_ED, it is a mated edge */
			if( e >= DCL_ED ) continue;

			printf("%d -> %d   ", edges[e].node, edges[mate(e)].node);
		}
		printf("\n");
		if( nodes[n].primOut > 0 )
			printf("Primary output %d\n", nodes[n].primOut);
	}
}

void GraphTransform()
{
	
        int n, n1, n2, e;
        for( e = 1; e <= edge_num; e++ ) {
                n1 = edges[e].node;
                n2 = edges[mate(e)].node;
                
                addEdge( e, n1, n2 );
	}

	OutputGraph();
}

