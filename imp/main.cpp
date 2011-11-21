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

	cout<<"Heuristic, "<<"RMQ, "<<"Serie, "<<"t, "<<"d, "<<"ans, "<<"min, "<<"med, "<<"max, "<<"av_time"<<endl;
	DataHandler *data_set;
	if (use_rdataset){
			data_set = new RandomDataHandler();
			data_set->load_dataset("/home/cvalentim/Mestrado/research/data_structures/instances/random/");
			cout<<"HAHA"<<endl;
	}	
	else{
			data_set = new RealDataHandler();
			data_set->load_dataset("/home/cvalentim/Mestrado/research/data_structures/instances/real_data/");
	}
	while (1){
		vector<double> A = data_set->get_next();
		if (A.empty()) break;
		Heuristic<double> *h;
		if (use_rmqH)
			h = new PureRmq<double>(); 
		else 
			h = new FPairs<double>();
		Executer<double> *env = new Executer<double>(h, 10, 10);
		env->go(A);
		cout<<endl;
	}

}
