
#include "dcl.h"
#include "utility.h"

void InitResource();


void ASAP()
{
	Queue Q;

	Q.init();

	int nd, e;

	for(nd = 1; nd <= nd_max; nd++) {
		// initialize temporary in/out degree
		node[nd].in_dgr_tmp = node[nd].in_degree;
		node[nd].out_dgr_tmp = node[nd].out_degree;
		node[nd].asap = 0;

		if( node[nd].in_dgr_tmp == 0 ) {
			Q.insertElement(nd);
			node[nd].asap = 1;
		}
	}

	latency_min = 0;
	while( !Q.isEmpty() ) {
		nd = Q.getElement();

		// decrease the degrees of all its successor nodes by one
		for(e = node[nd].edge; e > 0; e = edge[e].next) {
			if( e > ed_max ) continue;

			int n_s = edge[mate(e)].node;
			assert( node[n_s].in_dgr_tmp >= 1);

			node[n_s].in_dgr_tmp--;
			maximize(node[n_s].asap, node[nd].asap + 1);
			maximize(latency_min, node[n_s].asap);

			if( node[n_s].in_dgr_tmp == 0 ) {
				Q.insertElement(n_s);
			}
		}
	}
}


void ALAP()
{
	Queue Q;

	Q.init();

	int nd, e, ts;

	for(nd = 1; nd <= nd_max; nd++) {
		// initialize temporary in/out degree
		node[nd].in_dgr_tmp = node[nd].in_degree;
		node[nd].out_dgr_tmp = node[nd].out_degree;
		node[nd].alap = nd_max;

		if( node[nd].out_dgr_tmp == 0 ) {
			Q.insertElement(nd);
			node[nd].alap = nd_max;
		}
	}

	ts = nd_max;
	while( !Q.isEmpty() ) {
		nd = Q.getElement();

		// decrease the degrees of all its predecessor nodes by one
		for(e = node[nd].edge; e > 0; e = edge[e].next) {
			if( e <= ed_max ) continue;

			int n_s = edge[mate(e)].node;
			assert( node[n_s].out_dgr_tmp >= 1);

			node[n_s].out_dgr_tmp--;
			minimize(node[n_s].alap, node[nd].alap - 1);
			minimize(ts, node[n_s].alap);

			if( node[n_s].out_dgr_tmp == 0 ) {
				Q.insertElement(n_s);
			}
		}
	}


	for(nd = 1; nd <= nd_max; nd++) {
		node[nd].alap = node[nd].alap - ts + 1;
	}
}




/// Operator ready list for all kinds of FUs
ReadyList ready_op[FU_DCL];


void LS_initilization()
{
	int fu, op;
	int nd;

	for(nd = 1; nd <= nd_max; nd++) {
		// initialize temporary in/out degree
		node[nd].in_dgr_tmp = node[nd].in_degree;
		node[nd].out_dgr_tmp = node[nd].out_degree;
		// initialize the scheduling and FU allocation of all ops
		node[nd].sched_ts = 0;
		node[nd].FU_alloc = 0;
	}


	// initialize ready operator lists and variables for ADD and MUL
	for(fu = 1; fu <= FU_num; fu++) {
		ready_op[fu].init();
	}
}


void collectReadyOP()
{
	int nd, i, FU;

	for(nd = 1; nd <= nd_max; nd++) {
		if( node[nd].in_dgr_tmp == 0 ) {
			FU = node[nd].op;
			ready_op[FU].attachElement( nd, 1 );
		}	
	}
}


void updateReadyOP( int nd, int ts )
{
	// decrease the degrees of all its successor nodes by one
	int e, FU;
	for(e = node[nd].edge; e > 0; e = edge[e].next) {
		if( e > ed_max ) continue;

		int n_s = edge[mate(e)].node;
		assert( node[n_s].in_dgr_tmp >= 1);

		node[n_s].in_dgr_tmp--;
		
		if( node[n_s].in_dgr_tmp == 0 ) {
			FU = node[n_s].op;
			ready_op[FU].attachElement( n_s, ts );
		}
	}
}


void printAllResource()
{
	for(int FU_id = 1; FU_id <= FU_num; FU_id++) {
		printf("FU_%d: %d  ", FU_id, FUs[FU_id].totalInstNum());
	}
	printf("\n");
}

void collectUsedResource()
{

	energy_local = 0;
	for(int FU_id = 1; FU_id <= FU_num; FU_id++) {
//		printf("FU_%d: %d  ", FU_id, FUs[FU_id].usedInstNum());

		FU_inst_local[FU_id] = FUs[FU_id].usedInstNum();
		energy_local += FUs[FU_id].usedInstNum() * FUs[FU_id].getEnergy();
	}
//	printf("\nTotal Energy: %d\n", energy);
}




int ListSched( int LAT )
{

	int ts;
	int op_done;
	int op_idx, op_id;
	int fu;
	int idx;
	int FU_id;


	int sched_op[ND];
	int sched_op_num;


	// Initilizations
	LS_initilization();
	collectReadyOP();

	//printf("All available Resources:\n");
	//printAllResource();

	// Start to schedule
	op_done = 0;
	ts = 0;
	
	while( op_done < nd_max )
	{
		ts++;

		if( ts > LAT ) {
			//printf("Latency constraint not met (LAT: %d, Achieved: %d)\n", LAT, ts);
			//printf("Consider to increase the number of resources.\n");
			return 0;
		}

		//printf("\n=============== Time Slot %d\n", ts);

		// Assign operators according to their FU types
		for(FU_id = 1; FU_id <= FU_num; FU_id++) {

			FUs[FU_id].freeAllInst();
			sched_op_num = 0;
			while( FUs[FU_id].instAvailable() ) {

				op_idx = ready_op[FU_id].getRandEleIdx();
				// no ready op
				if( op_idx == 0) {
					break;
				}
				op_id = ready_op[FU_id].getRandEleVal( op_idx );

				// schedule op to time slot ts and assign a FU to it
				node[op_id].sched_ts = ts;
				node[op_id].FU_alloc = FUs[FU_id].assignInst();

				sched_op_num++;
				sched_op[sched_op_num] = op_id;

				// remove op from the ready list
				ready_op[FU_id].detachElement( op_idx );

				op_done++;

			}
			// update the degree of related nodes, attach them into ready list if ready
			for(idx = 1; idx <= sched_op_num; idx++) {
				updateReadyOP( sched_op[idx], ts+1 );
			}

			//ready_op[FU_id].printList();

		}
	}

	return ts;
}




int RemainingOP[FU_DCL];

void collectRemainingOP()
{
	
	for(int FU_id = 1; FU_id <= FU_num; FU_id++) {
		RemainingOP[FU_id] = 0;
	}
	
	for(int nd = 1; nd <= nd_max; nd++) {
		if(node[nd].sched_ts == 0) {
			RemainingOP[ node[nd].op ]++;
		}
	}

	//printf("Remaining OPs:\n");
	//for(int i = 1; i <= FU_num; i++) {
	//	printf("OP of type %d: %d, ", i, RemainingOP[i]);
	//}
	//printf("\n");
}



void printSchedBind()
{
	for(int nd = 1; nd <= nd_max; nd++ ) {
		printf("Node %d (%d): sched to TS %d, assigned to inst %d of FU %d\n",
			nd, node[nd].op, node[nd].sched_ts, node[nd].FU_alloc, node[nd].op);
	}
}


void printSchedBind_final()
{
	for(int nd = 1; nd <= nd_max; nd++ ) {
		printf("Node %d (%d): sched to TS %d, assigned to inst %d of FU %d\n",
			nd, node[nd].op, node[nd].sched_ts_final, node[nd].FU_alloc_final, node[nd].op);
	}
	
	for(int FU_id = 1; FU_id <= FU_num; FU_id++) {
		printf("FU_%d: %d  ", FU_id, FUs[FU_id].usedInstNumFinal());
	}
	printf("Total Energy Final: %d\n", energy_global);
}



void printUsedResource()
{
	int energy = 0;
	for(int FU_id = 1; FU_id <= FU_num; FU_id++) {
		printf("FU_%d: %d  ", FU_id, FUs[FU_id].usedInstNum());
		energy += FUs[FU_id].usedInstNum() * FUs[FU_id].getEnergy();
	}
	printf("\nTotal Energy: %d\n", energy);
}



void Sched_Bind( int iter )
{
	ASAP();
	ALAP();

	int ts;
	float adj_res;
	int FU_id;

	//for(int nd = 1; nd <= nd_max; nd++) {
	//	printf("Node %d's [ASAP, ALAP] scheduling: [%d, %d]\n", nd, node[nd].asap, node[nd].alap);
	//}

	// initialize the total number of ADDs and MULs (FU)
	InitResource();

	
	LAT = latency_min * 1.5;
	printf("latency_min: %d\n", latency_min);
	printf("Latency Constraint: %d\n", LAT);
	while( 1 ) {
		ts = ListSched(LAT);

		if( ts > 0 ) {

			// list scheduling and resource assignment succeeded
			LAT_achieved = ts;

			printf("\n\n---- Iteration %d ----\n", iter);
			printf("Achieved Latency: %d\n", LAT_achieved);
			printUsedResource();
			//printSchedBind();

			collectUsedResource();

			break;
		}
		else {

			collectRemainingOP();
             
			// First, if actually used FU instances are smaller than allocated ones
			// decrease the number of allocated FU instances
			for(FU_id = 1; FU_id <= FU_num; FU_id++) {
				if( FUs[FU_id].usedInstNum() < FUs[FU_id].totalInstNum() ) {
					//printf("FU_%d:  Used: %d, total: %d\n", FU_id, FUs[FU_id].usedInstNum(), FUs[FU_id].totalInstNum());
					FUs[FU_id].setTotalInstNum( FUs[FU_id].usedInstNum() );
				}
			}

			// increase the number of resources by chance
			adj_res = (float)rand() / (float)RAND_MAX;
			//printf("%f\n", adj_res);
			if( adj_res > 0.8 ) {
				FU_id = (rand() % FU_num) + 1;
				while( RemainingOP[FU_id] == 0 ) {
					FU_id = (rand() % FU_num) + 1;
				}
				//printf("increase %d FU by one\n", FU_id);
				FUs[FU_id].instIncrease();
			}
		}
	}
}



void saveSolution()
{	
	for(int nd = 1; nd <= nd_max; nd++ ) {
		node[nd].sched_ts_final = node[nd].sched_ts;
		node[nd].FU_alloc_final = node[nd].FU_alloc;
	}

	for(int FU_id = 1; FU_id <= FU_num; FU_id++) {
		FUs[FU_id].setUsedInstNumFinal( FUs[FU_id].usedInstNum() );
	}

	energy_global = energy_local;
}