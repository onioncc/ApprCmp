#include "utility.h"


///////////////////////// FUNCTIONS ////////

unsigned long long timeGetTime()
{
        struct timeval tv;
        gettimeofday( &tv, NULL );

        return tv.tv_sec*1000000 + tv.tv_usec;
}



//////////////// CLASS QUEUE ///////////////////
void Queue::init()
{
	front = 0;
	rear = -1;
	itemCount = 0;
}


bool Queue::isEmpty() {
   return itemCount == 0;
}


bool Queue::isFull() {
   return (rear+1) == QUEUE_MAX;
}

int Queue::getSize() {
   return itemCount;
}  

int Queue::insertElement(int ele)
{

	if( !isFull() ) {
		eleArray[ ++rear ] = ele;
		itemCount++;

		return 1;
	}
	else {
		printf("Queue is full. Element insertion failed.\n");
		return -1;
	}
}

int Queue::getElement()
{
	int ele; 
	
	if( !isEmpty() ) {
		if(front+1 == QUEUE_MAX) {
			printf("Queue is used up. Re-initialize the queue: all elements are cleared up.\n");
			init();
			return -1;
		}
		ele = eleArray[front++];
		itemCount--;
		return ele;  
	}
	else {
		printf("Queue is empty. Getting data failed.\n");
		return -1;
	}

}



//////////////// CLASS READYLIST ///////////////////

void ReadyList::init()
{
	head = 0;
	tail = 1;

	for(int i = 0; i < ND; i++) {
		ready_op[i].node = 0;
		ready_op[i].next = -1;
		ready_op[i].prev = -1;
		ready_op[i].ready_time = 0;
	}

	// head node
	ready_op[0].next = 1;
	ready_op[0].prev = -1;

	// tail node
	ready_op[1].next = -1;
	ready_op[1].prev = 0;

}



bool ReadyList::isEmpty()
{	
	if( ready_op[head].next == tail && ready_op[tail].prev == head )
		return 1;
	else
		return 0;

}



void ReadyList::attachElement( int ele, int ready_ts )
{
	ready_op[tail].node = ele;
	ready_op[tail].ready_time = ready_ts;
	ready_op[tail].next = tail + 1;
	ready_op[tail].prev = ready_op[tail].prev;
	
	tail++;
	ready_op[tail].node = 0;
	ready_op[tail].next = -1;
	ready_op[tail].prev = tail - 1;
}



void ReadyList::detachElement( int index )
{
	assert( isEmpty() != 1 );

	int prev = ready_op[index].prev;
	int next = ready_op[index].next;
	
	ready_op[prev].next = ready_op[index].next;
	ready_op[next].prev = ready_op[index].prev;

	ready_op[index].node = 0;
	ready_op[index].next = -1;
	ready_op[index].prev = -1;

}



int ReadyList::getRandEleIdx()
{
	int ready_num;
	int idx;
	int pick;

	ready_num = 0;
	for(idx = ready_op[head].next; ready_op[idx].node > 0; idx = ready_op[idx].next) {
		ready_num++;
	}

	if( ready_num == 0 ) {
		return 0;
	}

	unsigned long long t = timeGetTime();
	srand((unsigned)t);
	
	pick = rand() % ready_num;

	for(idx = ready_op[head].next; pick > 0; idx = ready_op[idx].next) {
		pick--;
	}

	assert( idx > 0 );

	return idx;
}


int ReadyList::getRandEleVal( int index )
{
	return ready_op[index].node;
}



void ReadyList::printList()
{
	printf("head: %d, tail: %d\n", head, tail);
	for(int idx = ready_op[head].next; ready_op[idx].node > 0; idx = ready_op[idx].next) {
		printf("node: %d (idx: %d) ", ready_op[idx].node, idx);
	}
	printf("\n");
}





//////////////// CLASS FUNCUNIT ///////////////////

void FuncUnit::init(int t, int e = 1000, int d = 1, int a = 1000)
{
	num_inst_total = 1;
	num_inst_avail = 1;
	num_inst_used = 0;

	type = t;
	energy = e;
	delay = d;
	area = a;

	resetAllInst();
}


void FuncUnit::resetAllInst()
{
	num_inst_avail = num_inst_total;

	for(int i = 1; i <= num_inst_total; i++) {
		instances[i].available = 1;
	}
}



void FuncUnit::instIncrease()
{
	num_inst_total++;
}

int FuncUnit::instDecrease()
{
	if( num_inst_total == 0) {
		printf("Already has zero instances.\n");
		return -1;
	}

	num_inst_total--;
	return num_inst_total;
}


bool FuncUnit::instAvailable()
{
	if( num_inst_avail > 0)
		return 1;
	else return 0;
}



int FuncUnit::assignInst()
{
	int i, used;

	// find the first available instance
	for(i = 1; instances[i].available == 0; i++ );

	instances[i].available = 0;
	num_inst_avail--;

	used = num_inst_total - num_inst_avail;
	num_inst_used = (num_inst_used < used) ? used : num_inst_used;


	return i;
}


int FuncUnit::availInstNum()
{
	return num_inst_avail;
}

int FuncUnit::totalInstNum()
{
	return num_inst_total;
}

int FuncUnit::usedInstNum()
{
	return num_inst_used;
}

int FuncUnit::setTotalInstNum(int num)
{
	num_inst_total = num;
}



