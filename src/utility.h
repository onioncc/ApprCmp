
#ifndef _UTILITY_H_
#define _UTILITY_H_


#include "dcl.h"


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


/////////////////////////////////////
//////// FUNCTIONS //////////////////

unsigned long long timeGetTime();



#endif