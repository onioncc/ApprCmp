
#ifndef _CLASSES_H_
#define _CLASSES_H_


#include "dcl.h"

#ifdef MAIN
	#define EXT
#else
	#define EXT extern
#endif


/////////////////////////////////////
//////// CLASSES ////////////////////

class Queue
{
public:
	bool isFull();
	bool isEmpty();
	void init();
	int getSize();
	int insertElement( int ele );
	int getElement();

private:
	int eleArray[QUEUE_MAX];
	int front;
	int rear;
	int itemCount;	
};



struct Ready_OP
{
	int node;
	int next;
	int prev;
	int ready_time;
};


class ReadyList
{
public:
	void init();
	bool isEmpty();
	void attachElement( int ele, int ready_ts );
	void detachElement( int index );
	int getRandEleIdx();
	int getRandEleVal( int index );
	void printList();

private:
	int head;
	int tail;
	Ready_OP ready_op[ND];
};





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
    int  instDecrease();
    bool instAvailable();
    int  assignInst();
    void freeAllInst();
    int  availInstNum();
    void setTotalInstNum(int num);
    int  totalInstNum();
    int  usedInstNum();
    int  usedInstNumFinal();
    void setUsedInstNumFinal(int num);
    int  getEnergy();
private:
    int type;   // 1: ADD; 2: MUL
    int energy;
    int delay;
    int area;   // use LUTs to represent area in FPGA
    int num_inst_total; // total number of instances of this FU type
    int num_inst_avail; // currently available number of instances
    int num_inst_used;  // actually used instances
    int num_inst_used_final;  // actually used instances
    FUInst instances[FU_DCL];
};



/////////////////////////////////////
//////// FUNCTIONS //////////////////

unsigned long long timeGetTime();


// number of FUs (not instances)
EXT int FU_num;
EXT FuncUnit FUs[FU_DCL];


#endif