#ifndef __EXECUTER__
#define __EXECUTER__

#include "timer/clock.cpp"
#include "heuristics/heuristic.cpp"

#include <vector>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <iomanip>

using namespace std;

template<class T>
class Executer{
	int nq;
	int nr;
	Heuristic<T> *h;

	void csv_printer(vector<T>& A, long long ans, int t, double d, vector<double>& times){
		sort(times.begin(), times.end());
		cout<<left;
		cout<<h->get_name()<<", "<<setw(6);
		cout<<A.size()<<", "<<setw(6);
		cout<<t<<", "<<setw(6); 
		cout<<setprecision(5)<<d<<", "<<setw(6);
		//cout<<d<<", ";
		cout<<ans<<", "<<setw(6);
		cout<<accumulate(times.begin(), times.end(), 0.0)/times.size()<<", "<<setw(6);
		cout<<times[0]<<", "<<setw(6);
		cout<<times[times.size()/2]<<", "<<setw(6);
		cout<<times.back()<<endl;
	}
		
public:
	Executer(Heuristic<T> *_h, int _nq, int _nr): h(_h), nq(_nq), nr(_nr){}
	void go(vector<T>& A){
			Clock clock = Clock();
			
			// timing the preprocessing time
			clock.start();
			h->preprocess(A);
			clock.end();

			int t = 1;
			double d = 1;
			for (int query = 0; query < nq; ++query){
					if (t > A.size()) break;
					t *= 2;
					d *= 1.5;
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
					csv_printer(A, ans, t, d, times);
			}
	}
};
#endif
