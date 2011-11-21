#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <algorithm>

#include "heuristics/pure_rmq.cpp"
#include "heuristics/fpairs.cpp"
//#include "heuristics/sort_heuristic.cpp"
#include "data_set_handler.cpp"
#include "executer.cpp"

using namespace std;

int main()
{
	cout<<"Heuristic, "<<"RMQ, "<<"Serie, "<<"t, "<<"d, "<<"ans, "<<"min, "<<"med, "<<"max, "<<"av_time"<<endl;
	DataHandler *data_set = new RealDataHandler();
	//data_set->load_dataset("/home/cvalentim/Mestrado/research/data_structures/instances/random/");
	data_set->load_dataset("/home/cvalentim/Mestrado/research/data_structures/instances/real_data/");
	while (1){
		vector<double> A = data_set->get_next();
		if (A.empty()) break;
		Heuristic<double> *h = new FPairs<double>();
		//Heuristic<double> *h = new PureRmq<double>();
		//SortHeuristic h = SortHeuristic();
		Executer<double> *env = new Executer<double>(h, 10, 10);
		env->go(A);
		cout<<endl;
	}

}
