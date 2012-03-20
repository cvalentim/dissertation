#ifndef __EXECUTER__
#define __EXECUTER__


#include <vector>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <unistd.h>


#include "timer/clock.cpp"
#include "heuristics/heuristic.cpp"
#include "heuristics/beginnings/range_list/range_list.cpp"
#include "data_set_handler.cpp"

using namespace std;

#define SMALL_OUTPUT 0
#define LARGE_OUTPUT 1

template<class T>
class Executer{
	void print_csv_header(){
		cout<<left;	
		cout<<"SerieID, "<<setw(6);
		cout<<"Heuristic, "<<setw(6);
		cout<<"Serie, "<<setw(6);
		cout<<"t, "<<setw(6);
		cout<<"d, "<<setw(6);
		cout<<"ans, "<<setw(6);
		cout<<"av_time, "<<setw(6);
		cout<<"min, "<<setw(6);
		cout<<"med, "<<setw(6);
		cout<<"max, "<<setw(6);
		cout<<"ends"<<setw(6);
		cout<<endl;
	}

	void csv_printer(int id_serie, Heuristic<T> *h, vector<T>& A, long long ans, int t, double d, vector<double>& times){
		sort(times.begin(), times.end());
		cout<<left;
		cout<<id_serie<<", "<<setw(6);
		cout<<h->get_name()<<", "<<setw(6);
		cout<<A.size()<<", "<<setw(6);
		cout<<t<<", "<<setw(6); 
		cout<<fixed<<setprecision(1)<<d<<", "<<setw(6);
		cout<<ans<<", "<<setw(6);
		cout<<accumulate(times.begin(), times.end(), 0.0)/times.size()<<", "<<setw(6);
		cout<<fixed<<setprecision(2)<<times[0]<<", "<<setw(6);
		cout<<fixed<<setprecision(2)<<times[times.size()/2]<<", "<<setw(6);
		cout<<fixed<<setprecision(2)<<times.back();
		if (h->get_name().substr(0, 5) == "Range")
			cout<<", "<<((RangeList<T> *)h)->last_query_ends<<setw(2);
		else
			cout<<", -"<<setw(2);
		cout<<endl;
	}
		
public:
	
	void exec(DataHandler<T> *data_set, Heuristic<T> *h, int nq, int nr, int outsize){
			print_csv_header();
			int serie = 1;
			while (1){
					cout<<endl;
					vector<T> data = data_set->get_next();
					if (data.empty()) break;
					exec_q(serie, data, h, nq, nr, outsize);
					cout<<endl;		
					++serie;
			}
	}
	
	void exec_q(int id_serie, vector<T>& A, Heuristic<T> *h, int nq, int nr, int outsize){
			Clock clock = Clock();
			
			// timing the preprocessing time
			clock.start();
			h->preprocess(A);
			clock.end();
			cout<<clock.elapsed()<<endl;
			/*
 				NOTE: To generate the results from 12_dec
				start with t=4 and d = A.size() - 1000 then
				continue with t += 200 and d -= 20
			*/
			int t = 1;
			double d = 1.3;
			for (int query = 0; query < nq; ++query){
					if (t > A.size()) break;

					if (outsize == SMALL_OUTPUT){
						t += 2;
						d *= 1.2;
					} else if (outsize == LARGE_OUTPUT){
						// large output size
						t += 4;
						d *= 1.1;
					}

					vector<double> times;
					long long ans = -1;
					for (int r = 0; r < nr; ++r){
							clock.start();
							long long res = h->query(t, d);
							clock.end();
							// all repetitions must return the same
							assert (ans == -1 || res == ans);
							ans = res;
							times.push_back(clock.elapsed());
					}
					csv_printer(id_serie, h, A, ans, t, d, times);
			}
	}
};
#endif
