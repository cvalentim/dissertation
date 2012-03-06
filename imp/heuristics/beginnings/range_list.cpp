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

#ifndef __BEG__RANGE_LIST__
#define __BEG__RANGE_LIST__

#include <cassert> // for assert 
#include <set> // for set
#include <algorithm> //for sort
#include <iostream>
#include <queue>

#include "../../../../rmq/cpp/rmq_naive.cpp" // for an <O(n), O(\log n)> RMQ
#include "../../../../rmq/cpp/rmq_st.cpp" // for an <O(n \log n), O(1)> RMQ 
#include "../heuristic.cpp" // for the abstract heuristic class
#include "fpair.cpp"
#include "fpair_range_list.cpp"

using namespace std;

template<class T>
class RangeList;

template<class T>
struct lt_by_d : std::binary_function<int , int, bool>
{
   	lt_by_d(RangeList<T> *p):rangeList(p) {}

    bool operator() (const int& index1, const int& index2) {
				return rangeList->seq[index1] < rangeList->seq[index2];
    }
    RangeList<T> *rangeList;
};

template<class T>
class RangeList : public Heuristic<T>{
	// RMQ used to find the maximum in a given sequence
	// interval. Note that it works both for min and max,
	// depeding on the compare function given during 
	// the preprocessing and query answering. See below.
	RMQNaive<T> rmqMin;

	// a copy of the input sequence
	vector<T> seq;

	// count how many answers were found for the query
	long long ans;

	//
	RangeTreeFPair<T> rtree;

	friend class lt_by_d<T>;	
	
public:
	RangeList(){}

	// see the abstract heuristic class for
	// a definition of this function
	int get_presize(){
		return 42;
	}

	// horrible hard-coded method used
	// to discover the heuristic name and
	// type of RMQ being used
	string get_name(){
		return "RMQHeuristic, RMQNaive";
	}

	// this heuristic preprocessing is
	// simple, since it only needs a RMQ structure
	void preprocess(vector<T>& A)
	{
			seq = A;
			rmqMin.preprocess(A);
			vector<FPair<T> > fpairs;
			find_fpairs(seq, fpairs);
			//cout<<fpairs.size()<<endl;	
			rtree.range_preprocess(fpairs);
	}

	// Find all beggings within the interval [a, b) 
	// that form a pair with the ending 'end'. Returns
	// solutions returned through the beg parameter
	void findByEnd(int end, int a, int b, vector<int>& beg, T delta_v){
				assert (b <= end);
				if (a >= b) return;
				int m = rmqMin.query(a, b-1);
				if (seq[end] - seq[m] < delta_v) return;
				++ans;
				findByEnd(end, a, m, beg, delta_v);
				findByEnd(end, m + 1, b, beg, delta_v);
	}

	vector<int> BegByEnd(vector<int>& E, int delta_t, T delta_v){
				// for this strategy to work we must process the
				// intervals from the greatest to the smallest
				sort(E.rbegin(), E.rend(), lt_by_d<T>(this));

				// this set keeps endings already processed	
				set<int> covered;

				// this function return, all the beginings
				// we find through the endings we have
				vector<int> beginnings;

				for (vector<int>::iterator it = E.begin(); it != E.end(); ++it){
							// the current it(end) can find new beginings within the interval [a, b)
							int b = *it;
							int a = max(*it - delta_t, 0);
							// now we try to prune the interval [a, b)
							// using the fact that the current end doesn't have to 
							// explore regions already explored by previous endings
							set<int>::iterator post = covered.lower_bound(*it);
							
							if (post != covered.end())
									b = min(b, *post - delta_t);
							if (post != covered.begin())
									a = max(a, *(--post));
							if (b <= 0 || b <= a)	
									continue;
							findByEnd(*it, a, b, beginnings, delta_v);
							covered.insert(*it);
				}
				return beginnings;				
	}

	vector<int> BegByEnd2(vector<int>& E, int delta_t, T delta_v){
			vector<int> beg;
			// sort in decreasing index order
			E.push_back(0);
			sort(E.begin(), E.end());
			deque<int> Q;
			Q.push_front(E[E.size() -1]);
			int currHigh = Q.front();
			for (int i = E.size() - 2; i >= 0; --i){
						int e = Q.front();
						while (e - delta_t >= E[i]){
								findByEnd(e, e - delta_t, currHigh, beg, delta_v);
								Q.pop_front();
								currHigh = e - delta_t; 
								if (Q.empty()) {
											e = -1;
											break;
								}
								e = Q.front();
						}
						if (e != -1)
							findByEnd(e, E[i], currHigh, beg, delta_v);
						while (!Q.empty() && seq[Q.back()] < seq[E[i]]) Q.pop_back();
						Q.push_back(E[i]);
						currHigh = E[i];						
			}
			return beg; 
	}

	long long query(int delta_t, T delta_v){
			ans = 0;
			vector<int> beg = _query(delta_t, delta_v);
			assert (beg.size() == 0);
			return ans;
	}

	vector<int> _query(int delta_t, T delta_v){
		// only positive deltas
		assert (delta_t > 0);
		vector<int> endings;
		rtree.range_query(delta_t, delta_v, endings);
		//cout<<"#fins = "<<endings.size()<<endl;
		return BegByEnd2(endings, delta_t, delta_v);
	}

};
#endif
