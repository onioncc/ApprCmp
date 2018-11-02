

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

using namespace std;


#ifdef MAIN
	#define EXT
#else
	#define EXT extern
#endif


#ifdef DEBUG
#define ED		100
#define ED1		2*ED+1
#define ND		200
#define TS_MAX	ED
#define ADD_DCL	50
#define MUL_DCL	50
#define FU_DCL  20
#else
#define ED		10000
#define ED1		2*ED+1
#define ND		20000
#define TS_MAX	ED
#define ADD_DCL	500
#define MUL_DCL	500
#define FU_DCL  20
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
} Edge;


typedef struct {
    int		op;	// 1: ADD; 2: MUL
    int		in_degree;
    int		out_degree;
    int		in_dgr_tmp;
    int		out_dgr_tmp;
    int		edge;
    int		asap;
    int		alap;
    int 	sched_ts;
    int 	priority;
    int 	FU_alloc;
 } Node;


typedef struct {
	int 	available;
} ADD_FU;


typedef struct {
	int 	available;
} MUL_FU;


class FUInst    // functional unit instances
{
public:
    bool available;
};


class FuncUnit  // functional unit definition
{
public:
    void init(int type, int energy, int delay, int area);
    void instIncrease();
    int instDecrease();
    bool instAvailable();
    int assignInst();
    void resetAllInst();
    int availInstNum();
    int setTotalInstNum(int num);
    int totalInstNum();
    int usedInstNum();
private:
    int type;   // 1: ADD; 2: MUL
    int energy;
    int delay;
    int area;   // use LUTs to represent area in FPGA
    int num_inst_total; // total number of instances of this FU type
    int num_inst_avail; // currently available number of instances
    int num_inst_used;  // actually used instances
    FUInst instances[FU_DCL];
};



/////////////// Global Variables ///////////

// edges and nodes of the DFG
EXT Edge edge[ED1+1];
EXT Node node[ND+1];

EXT int nd_max;
EXT int ed_max;


// number of FUs (not instances)
EXT int FU_num;
EXT FuncUnit FUs[FU_DCL];

// number of final FU instances of each type
EXT int FU_inst_global[FU_DCL];
EXT int FU_inst_local[FU_DCL];


// latency constraint
EXT int latency_min;	//minimum latency on critical path
EXT int LAT;	// latency constraint
EXT int LAT_achieved;



#endif  