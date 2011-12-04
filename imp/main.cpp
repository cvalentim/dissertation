#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <algorithm>
#include <cstring>

#include "heuristics/pure_rmq.cpp"
#include "heuristics/fpairs.cpp"
#include "data_set_handler.cpp"
#include "executer.cpp"

using namespace std;

void help(){
	cout<<"./<program> <REAL_DATASET | RANDOM_DATASET> <FPAIR_H | RMQ_H>"<<endl;
	cout<<"REAL_DATASET -> It uses the real instances available on /instances/real_data"<<endl;
	cout<<"RANDOM_DATASET -> It uses random instances available on /instances/random"<<endl;
	cout<<"FPAIR -> It uses the FPairs heuristic to answer the queries"<<endl;
	cout<<"RMQ_H -> It uses the RMQ_H heuristic to answer the queries"<<endl;
	exit(0);
}

int main(int argc, char* argv[])
{
	bool use_rdataset = false, use_rmqH = false;
	if (argc < 2) {
			help();
	}

	if (strcmp(argv[1], "RANDOM_DATASET") == 0)	use_rdataset = true;
	else if (strcmp(argv[1], "REAL_DATASET") == 0) use_rdataset = false;
	else help();
	
	if (strcmp(argv[2], "RMQ_H") == 0) use_rmqH = true;
	else if (strcmp(argv[2], "FPAIR_H") == 0) use_rmqH = false;
	else help();

	if (use_rdataset){
			DataHandler<int> *data_set = new RandomDataHandler<int>();
			data_set->load_dataset("/home/cvalentim/Mestrado/research/data_structures/instances/random/");
			Heuristic<int>* h;
			if (use_rmqH)
				h = new PureRmq<int>();
			else
				h = new FPairs<int>();
			Executer<int> *env = new Executer<int>();
			env->exec(data_set, h, 30, 5);
	}	
	else{
			DataHandler<double> *data_set = new RealDataHandler<double>();
			data_set->load_dataset("/home/cvalentim/Mestrado/research/data_structures/instances/real_data/");
			Heuristic<double> *h;
			if (use_rmqH)
				h = new PureRmq<double>();
			else
				h = new FPairs<double>();
			Executer<double> *env = new Executer<double>();
			env->exec(data_set, h, 30, 5);
	}
	return 0;
}
