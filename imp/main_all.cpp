#ifndef __MAIN_ALL__
#define __MAIN_ALL__

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <algorithm>
#include <cstring>

#include "heuristics/allpairs/h_fpairs.cpp"
#include "heuristics/allpairs/pure_rmq.cpp"
#include "heuristics/heuristic.cpp"
#include "data_set_handler.cpp"
#include "executer.cpp"

using namespace std;

#define RMQ_BUCKET 0
#define RMQ_ST 1
#define HFPAIRS 2

void help(){
	cout<<"./<program> <OUTSIZE_SMALL | OUTSIZE_LARGE> <RMQ_BUCKET | HFPAIRS | RMQ_ST>"<<endl;
	cout<<"<OUTSIZE_SMALL | OUTSIZE_LARGE > -> controls the size of query answers"<<endl;
	cout<<"RMQ_BUCKET -> Uses the RMQ based heuristic with the RMQBucket implementation for the RMQ."<<endl;
	cout<<"RMQ_ST -> it uses the RMQ based heuristic backed by the <O(nlogn), O(1)> implementation of RMQ."<<endl;
	cout<<"HFPAIRS -> uses the heuristic based on FPairs."<<endl;
	exit(0);
}

int main(int argc, char* argv[])
{
	int type_h, outsize, onlyPre = 0;

	if (argc < 2) {
			help();
	}

	if (strcmp(argv[1], "OUTSIZE_SMALL") == 0) outsize = 0;
	else if (strcmp(argv[1], "OUTSIZE_LARGE") == 0) outsize = 1;
	else if (strcmp(argv[1], "PRE") == 0) onlyPre = 1; 
	else help();

	if (strcmp(argv[2], "RMQ_BUCKET") == 0) type_h = RMQ_BUCKET;
	else if (strcmp(argv[2], "HFPAIR") == 0) type_h = HFPAIRS;
	else if (strcmp(argv[2], "RMQ_ST") == 0) type_h = RMQ_ST;
	else help();

	DataHandler<double> *data_set = new RealDataHandler<double>();
	data_set->load_dataset("/home/cvalentim/Mestrado/research/data_structures/instances/real_data/data/");

	Heuristic<double> *h;
	switch (type_h){
		case RMQ_BUCKET:
			h = new PureRmq<double>(new RMQBucket<double>(RMQTypes_t::MAX()));
			break;
		case RMQ_ST:
			h = new PureRmq<double>(new RMQSt<double>(RMQTypes_t::MAX()));
			break;
		case HFPAIRS:
			h = new HFPairs<double>();
			break;
	}
	Executer<double> *env = new Executer<double>();
	if (onlyPre)
		// repeat 15 the preprocessing fase
		env->execPre(data_set, h, 10);
	else
		// execute 15 queries and repeat each query five times
		env->exec(data_set, h, 15, 10, outsize);
	return 0;
}
#endif
