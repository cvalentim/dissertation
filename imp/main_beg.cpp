#ifndef __MAIN_BEG__
#define __MAIN_BEG__

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <algorithm>
#include <cstring>

#include "heuristics/beginnings/range_list/range_list.cpp"
#include "heuristics/beginnings/rmq_based.cpp"
#include "heuristics/beginnings/allpairs_fpairs.cpp"
#include "heuristics/heuristic.cpp"
#include "data_set_handler.cpp"
#include "executer.cpp"

using namespace std;

#define RMQ_BUCKET 0
#define FPAIR 1
#define ALLPAIRS_FPAIR 2
#define RMQ_ST 3

void help(){
	cout<<"./<program> <OUTSIZE_SMALL | OUTSIZE_LARGE> <RMQ_BUCKET | FPAIR | ALLPAIRS_FPAIR | RMQ_ST>"<<endl;
	cout<<"<OUTSIZE_SMALL | OUTSIZE_LARGE > -> controls the size of query answers"<<endl;
	cout<<"RMQ_BUCKET -> Uses the RMQ based heuristic with the RMQBucket implementation for the RMQ."<<endl;
	cout<<"FPAIR -> It uses the FPairs heuristic to answer the queries"<<endl;
	cout<<"RMQ_ST -> it uses the RMQ based heuristic backed by the <O(nlogn), O(1)> implementation of RMQ."<<endl;
	cout<<"ALLPAIRS_FPAIR -> uses the fpairs heuristic from the AllPairs problem to	answer queries of the beginnings problem"<<endl;
	exit(0);
}

int main(int argc, char* argv[])
{
	int type_h, outsize;

	if (argc < 2) {
			help();
	}

	if (strcmp(argv[1], "OUTSIZE_SMALL") == 0) outsize = 0;
	else if (strcmp(argv[1], "OUTSIZE_LARGE") == 0) outsize = 1;
	else help();

	if (strcmp(argv[2], "RMQ_BUCKET") == 0) type_h = RMQ_BUCKET;
	else if (strcmp(argv[2], "FPAIR") == 0) type_h = FPAIR;
	else if (strcmp(argv[2], "ALLPAIRS_FPAIR") == 0) type_h = ALLPAIRS_FPAIR;
	else if (strcmp(argv[2], "RMQ_ST") == 0) type_h = RMQ_ST;
	else help();

	DataHandler<double> *data_set = new RealDataHandler<double>();
	data_set->load_dataset("/home/cvalentim/Mestrado/research/data_structures/instances/real_data/new_series_rev/");

	Heuristic<double> *h;
	switch (type_h){
		case 0:
			h = new RmqBased<double>(new RMQBucket<double>(RMQTypes_t::MAX()));
			break;
		case 1:
			h = new RangeList<double>();
			break;
		case 2:
			h = new AllPairsFPairs<double>();
			break;
		case 3:
			h = new RmqBased<double>(new RMQSt<double>(RMQTypes_t::MAX()));
			break;
	}
	Executer<double> *env = new Executer<double>();
	// execute 15 queries and repeat each query 50 times
	env->exec(data_set, h, 15, 5, outsize);
	return 0;
}
#endif
