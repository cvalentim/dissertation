#ifndef __PURE_RMQ__
#define __PURE_RMQ__

#include <iostream>
#include <cassert>

#include "../heuristic.cpp"
#include "../../../../rmq/cpp/rmq_bucket.cpp"
#include "../../../../rmq/cpp/rmq_st.cpp"
#include "../../../../rmq/cpp/abstract_rmq.cpp"


using namespace std;

template<class T>
class PureRmq : public Heuristic<T>{
	vector<T> seq;

    RMQ<T> *rmqMax;

	vector<pair<int, int> > ans;

	/* all pairs which matching the index f in the
		closed interval [s, e] */
	void calculate(int f, int s, int e, double d)
	{
		if (s > e) return;
		assert(0 <= f && f < seq.size());
		assert(0 <= s && s < seq.size());
		assert(0 <= e && e < seq.size());
		int m = rmqMax->query(s, e);
		if (seq[m] - seq[f] < d) return;
		ans.push_back(make_pair(f, m));
		calculate(f, s, m - 1, d);
		calculate(f, m + 1, e, d); 
	}
	
public:
	PureRmq(RMQ<T> *_rmqMax){
		rmqMax = _rmqMax;
	}

    ~PureRmq(){
        delete rmqMax;
    }

	int get_presize(){
		return 42;
	}

	string get_name(){
		return "Allpairs: RMQBased with " + rmqMax->name();
	}

	void preprocess(vector<T>& A)
	{
		seq = A;
		rmqMax->preprocess(seq);
	}

	vector<pair<int, int> > enumQuery(int t, T d){
		ans.clear();
		for (int i = 0; i < seq.size(); ++i){
			calculate(i, i + 1, min(i + t, (int)seq.size() - 1), d);
		}
		return ans;		
	}

	long long query(int t, T d){
		assert (t > 0);
		return (long long) enumQuery(t, d).size();
	}
};
#endif
