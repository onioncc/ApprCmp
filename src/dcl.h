

#ifndef _DCL_H_
#define _DCL_H_



#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>
#include <assert.h>
#include <iostream>
#include <fstream>

using namespace std;


#ifdef MAIN
	#define EXT
#else
	#define EXT extern
#endif


#ifdef DEBUG
#define ED		200
#define ED1		2*ED+1
#define ND		200
#define TS_MAX	ED
#define FU_DCL  50
#else
#define ED		20000
#define ED1		2*ED+1
#define ND		20000
#define TS_MAX	ED
#define FU_DCL  500
#endif


#define QUEUE_MAX   ED1

#define abs(a)          (((a) < (0.)) ? -(a) : (a))
#define minimize(x, val) if ((val) < (x)) (x) = (val)
#define maximize(x, val) if ((val) > (x)) (x) = (val)

#define max(a, b)       (((a) > (b)) ? (a) : (b))
#define min(a, b)       (((a) < (b)) ? (a) : (b))
#define mod(x, div)     ((x) - (x) / (div) * (div))
#define Round(x)        ((int)(x + 0.5 - (x<0)))

#define Top(e)  2*e-1
#define Bot(e)  2*e

inline int mate(int i) {
	return (ED1 - (i));
}


typedef struct {
	int		node;
	int		id;
    int		next;
    float 	DFG_value;
} Edge;


typedef struct {
    int		op;	// 1: ADD; 2: MUL; 3: SQRT
    int		in_degree;
    int		out_degree;
    int		in_dgr_tmp;
    int		out_dgr_tmp;
    int		edge;

    // for primary IO
    int 	prim_in1;
    int 	prim_in2;
    int 	prim_out;
    float 	prim_in1_val;
    float 	prim_in2_val;
    float 	prim_out_val;

    // for sched and bind
    int		asap;
    int		alap;
    int 	sched_ts;
    int 	priority;
    int 	FU_alloc;

    // final sched and bind solutions
    int 	sched_ts_final;
    int 	FU_alloc_final;
 } Node;



/////////////// Global Variables ///////////


// edges and nodes of the DFG
EXT Edge edge[ED1+1];
EXT Node node[ND+1];

EXT int nd_max;
EXT int ed_max;

EXT int prim_in_max;
EXT int prim_in_num;
EXT int prim_out_num;

// number of final FU instances of each type
EXT int FU_inst_local[FU_DCL];
EXT int FU_inst_global[FU_DCL];
EXT int energy_local;
EXT int energy_global;


// latency constraint
EXT int latency_min;	//minimum latency on critical path
EXT int LAT;	// latency constraint
EXT int LAT_achieved;



#endif  