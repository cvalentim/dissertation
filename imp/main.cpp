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

int main(int argc, char* argv[])
{
	bool use_rdataset = false, use_rmqH = false;
	if (argc > 1 && strcmp(argv[1], "RANDOM_DATASET") == 0)	use_rdataset = true;
	if (argc > 2 && strcmp(argv[2], "RMQ_H") == 0) use_rmqH = true;

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
