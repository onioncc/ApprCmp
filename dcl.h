#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>
#include <ctype.h>
#include <assert.h>


/************** white magic **********************************************/
#ifdef MAIN
  #define EXT
#else
  #define EXT extern
#endif

#define FIR_Input_Size		256
#define FIR_Output_Size		64

#define DCL_ED		10000
#define DCL_ND		20000
#define DCL_ED1		40000
#define DCL_OUT		100

inline int mate(int i) {
        return (DCL_ED1 - (i));
}


typedef struct {
	int id;
	int edge;	// edge
	int op;		// operator type; 1+, 2*, 3shift
	int primOut;	// primary output number; 0: non-primary; 1: output1, 2: output2, etc.
	int outValue;	// output value of the operator
	short int outValid;	// is output value valid; 00: non-valid; 01: left valid; 10: right valid; 11: both valid
	int addr1;	// address of input operand1. If addr1 == 0, oprand1 is a constant; otherwise OprdL is input xin[addr1]
	int addr2;	// address of input operand2. If addr2 == 0, oprand2 is a constant; otherwise OprdR is input xin[addr2]
	int OprdL;	// left operand
	int OprdR;	// right operand

	int FU_id;	// pysical FU id
	/* 0: accurate ADD or MUL */
	/* 1: LOA_add */
	/* 2: UDM_MUL */
}Node;

typedef struct {
	int id;
	int node;
	int next;
}Edge;



EXT int node_num;
EXT int edge_num;

EXT Node nodes[DCL_ND];
EXT Edge edges[DCL_ED1];

/* inputs for FIR: Xin and h[]*/
EXT int Xin[512];

EXT int inputNum;		// number of input data (primary inputs except for constants)
EXT int outputNum;		// number of primary outputs
EXT int accurOuts[DCL_OUT];	// accurate outputs
EXT int primOuts[DCL_OUT];	// primary outputs of DFG

/* error related */
EXT float errConstr;		// relative error constraint on primary outputs

EXT int InternalAddrMax;	// the maximum address of internal inputs
EXT int HAddrBase;		// the address base for H[] (first element, H[0])
EXT int XinAddrBase;		// the address base for Xin[] (first element Xin[0])


EXT int tbPtr;			// to indicate which testbench is running
/* testbench pointer values */
/* tbPtr = 1: FIR*/

