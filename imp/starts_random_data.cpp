#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <algorithm>

#include "heuristics/starts/starts_pure_rmq.cpp"
#include "heuristics/rmq_heuristic_count.cpp"
#include "heuristics/sort_heuristic.cpp"
#include "timer/clock.cpp"

using namespace std;

int main()
{

	for (int s = 1; s <= 5; ++s){ 	
		ifstream f;
		// string path = "/home/cvalentim/Mestrado/research/projeto_final_cpp/code/new_instances/serie";
		char path[1000];
		sprintf(path, "/home/cvalentim/Financial-Data-Structures/instances/random/random_serie%d.in", s);
		cout<<path<<endl;
		f.open(path);
		if (!f){
			cout<<"Could not open instance file."<<endl;
			return 1;	
		}
		vector<double> A;
		string value;
		double smallest = -1, biggest = -1;
		while (f >> value){
			double v = atof(value.c_str());	
			A.push_back(v);
			if (smallest == -1 || v < smallest) smallest = v;
			if (biggest == -1 || v > biggest) biggest = v;
		}
		f.close();

		cout<<"====================================="<<endl;
		cout<<"Serie "<<s<<": "<<endl;
		cout<<"Sequence length is "<<A.size()<<endl;
		cout<<"====================================="<<endl;
		cout<<endl;
		Clock clock = Clock();		

		//RMQHeuristicCount h = RMQHeuristicCount();
		SPureRmq h = SPureRmq();
		//SortHeuristic h = SortHeuristic();
	
		// preprocessing 
		clock.start();
		h.preprocess(A);
		clock.end();
		cout<<"Preprocessing Statistics: "<<endl;
		cout<<"Time = "<<clock.elapsed()<<" ms"<<endl;
		cout<<"Presize = "<<h.get_presize()<<endl;

	
		// queries
		double total_query_time = 0;
		double av_query_time;
		double av_size = 0;
		
		int nqueries = 10;
		int nrep = 10;

		cout<<endl;
		cout<<"Query Sequence:"<<endl;
		int t = 1;
		double d = A.size() - 1000;

		for (int query = 0; query < nqueries; ++query){
			t += 5;
			if (t > A.size()) {nqueries = query; break;}
			d -= 100;

			//d *= 2;
			//int t = rand()%A.size();
			//int t = rand()%201 + 100;
			//int d = rand()%A.size();
			//int d = A.size()/(rand()%201 + 100);
			long long ans = -1;
			double av_time = 0;
			for (int r = 0; r < nrep; ++r){
				clock.start();
				long long res = h.query(t, d);
				assert(ans == -1 || res == ans);
				ans = res;
				av_size += ans;
				clock.end();
				av_time += clock.elapsed();
			}
			av_time /= nrep;
			total_query_time += av_time;
			cout<<"t = "<<t<<" "<<"d = "<<d<<" ans = "<<ans<<" time = "<<av_time<<" ms"<<endl;
		}
		av_query_time = total_query_time/nqueries;
		av_size /= (nqueries * nrep);

		cout<<endl;
		cout<<"Query Statistics: "<<endl;
		cout<<"Number of queries = "<<nqueries<<endl;
		cout<<"Total query time = "<<total_query_time<<"ms"<<endl;
		cout<<"Average query time = "<<av_query_time<<"ms"<<endl;
		cout<<"Average answer size = "<<av_size<<endl;
		cout<<"Serie Length/Average size = "<<A.size()/av_size<<endl;
		cout<<endl<<endl;
	}

}
