#ifndef __EXECUTER__
#define __EXECUTER__


#include <stdio.h>
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
		if (h->get_name().substr(0, 6) == "Beg-SP")
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

	void execPre(DataHandler<T> *data_set, Heuristic<T> *h, int nrep){
		print_csv_header();
		int serie = 1;
		while (1){
			cout<<endl;
			vector<T> data = data_set->get_next();
			if (data.empty()) break;
			exec_p(serie, data, h, nrep);
			cout<<endl;		
			++serie;
		}
	}

	void exec_p(int id_serie, vector<T>& A, Heuristic<T> *h, int nrep){
		cerr<<"Measuring preprocessing time of "<<h->get_name()<<" with serie "<<id_serie<<" nrep = "<<nrep<<endl;
		Clock clock = Clock();
		// timing the preprocessing time
		double totalTime = 0;
		for (int i = 0; i < nrep; ++i){
			clock.start();
			h->preprocess(A);
			clock.end();
			totalTime += clock.elapsed();
		}
		cout<<"#pretime: "<<id_serie<<">"<<A.size()<<">"<<h->get_name()<<">"<<totalTime/nrep<<endl;
		// just to be sure the compiler will try to optimize and
		// forget about the loop above
		h->query(1, 1.3);
		h->query(10, 1.5);
		h->query(5, 1);
	}



	void exec_q(int id_serie, vector<T>& A, Heuristic<T> *h, int nq, int nr, int outsize){
		Clock clock = Clock();
		// timing the preprocessing time
		clock.start();
		h->preprocess(A);
		clock.end();
		//cout<<clock.elapsed()<<endl;
		cout<<"#pretime: "<<id_serie<<">"<<A.size()<<">"<<h->get_name()<<">"<<clock.elapsed()<<endl;
		int t = 1;
		double d = 1.3;
		// for the extended corpus of 100 series do:
		//double d = 0.1;
		for (int query = 0; query < nq; ++query){
			if (t > A.size()) {
				cout<<"t"<<"("<<t<<")"<<" is larger then the serie size. Aborting.";
				break;
			}
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
			// executes queries and generates time statistics
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
