/*
 * This code implements a heuristic for the Beginnings problem.
 * The ideia is to create a RMQ structure in the preprocessing stage and
 * use that structure to help on the query answering. A query is aswered
 * with only one scan over the sequence. During the scan we fix each
 * possible beginning and do an O(1) check to see if it is a valid 
 * beggining for the given (t, d) query. The check is pretty simple,
 * let's say we have fixed an element with index $i$ from the sequence,
 * it is valid beginning iff there is an index $j \in [i, i + t]$ such 
 * that $a_j - a_i \ge d$, so, it easy to see that we need only to consider
 * the bigest index (in terms of value) between $i$ and $i+t$, and, since
 * we have a RMQ structure for the sequence, we can find this index in $O(1)$.
 *
 * Runtime: <O(RMQ preprocessing time), O(n x RMQ query time)>
 *
 */

#ifndef __BEG__RMQBASED__
#define __BEG__RMQBASED__

#include <assert.h> // for assert 

#include "../heuristic.cpp" // for the abstract heuristic class
#include "../../../../rmq/cpp/abstract_rmq.cpp" 

using namespace std;

template<class T>
class RmqBased : public Heuristic<T>{
	// RMQ used to find the maximum in a given sequence
	// interval. Note that it works both for min and max,
	// depeding on the compare function given during 
	// the preprocessing and query answering. See below.
	RMQ<T> *rmqMax;

	// a copy of the input sequence
	vector<T> seq;

	// count how many answers were found for the query
	long long ans;
	
public:
	RmqBased(RMQ<T> *_rmqMax){
		rmqMax = _rmqMax;
	}

	~RmqBased(){
		delete rmqMax;
	}

	// see the abstract heuristic class for
	// a definition of this function
	int get_presize(){
		return 42;
	}

	string get_name(){
		return "Beg-RMQ(" + rmqMax->name() + ")";
	}

	// this heuristic preprocessing is
	// simple, since it only needs a RMQ structure
	void preprocess(vector<T>& A)
	{
		seq = A;
		// we want a max RMQ, so the 'greater' argument
		rmqMax->preprocess(seq);
	}

	long long query(int delta_t, T delta_v){
		// only positive deltas
		assert (delta_t > 0);
		ans = 0;
		int n = seq.size();
		// this loop fix each possible beginning 
		// and checks inside it if it is valid
		for (int i = 0; i < seq.size(); ++i){
			int m = rmqMax->query(i, (i + delta_t < n )? (i+delta_t): n - 1);
			if (seq[m] - seq[i] >= delta_v) ++ans;
		}
		return ans;
	}
};
#endif
