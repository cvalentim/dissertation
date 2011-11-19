#include <iostream>
#include <algorithm>
#include <set>
#include <assert.h>
#include <queue>
#include <stack>

#include "../rmq/rmq_st.cpp"

using namespace std;


class RMQHeuristicCount
{
	// =====================================================
    //  Preprocessing Structures
	// ===================================================

	// Our major structure, it is used to speed up the querying answering.
	// Given an input sequence {a_1, ..., a_n}, delta_time[t] has
	// the pair (a_{i + t} - a_i, i) if:
	//  - a_i < min(a_{i + 1}, ..., a_{i + t}) and
	//  - a_{i+t} > max(a_{i}, ..., a_{i + t -1})
	//
	// The expected size of delta_time is O(n).
	vector<vector<pair<double, int> > > delta_time;


	vector<RMQStMax> rmq_delta_time;

	// rmqs to construct efficiently the delta_time structure
	RMQStMin rmq_min;
	RMQStMax rmq_max;


	// the input sequence
	vector<double> seq; 

	// =====================================================
    //  Query Structures	
	// ===================================================


	// an id for the current query, it is useful to make hash without
	// cleaning the hash vector at each query
	long long nquery;

	// a hash to know which start or end is already on the queue
	vector<int> in_queue_s, in_queue_e;	

	// a hash to know which start (end) has already been processed (that is, all solutions involving this start/end were found)
	vector<int> hash_s, hash_e;	

	// for each query keep the stars/ends which yields at least one solution
	stack<pair<int, char> >* QStartEnd; 

	// for each time t, top[t] keeps the biggest interval difference in delta_time[t] 
	vector<double> top; 	

	// a rmq to quickly find lists with at least one solution
	RMQStMax rmq_top;

	// the number of found solutions
	long long ans;

	void construct(int s, int e){
		if (s >= e) return;
		int hi = rmq_max.query(s, e);
		int lo = rmq_min.query(s, hi);
		while (lo < hi && seq[lo] < seq[hi]){
			delta_time[hi - lo].push_back(make_pair(seq[hi] - seq[lo], lo));
			top[hi - lo] = max(top[hi - lo], seq[hi] - seq[lo]);
			lo = rmq_min.query(lo + 1, hi);		
		}
		construct(s, hi - 1);
		construct(hi + 1, e);
	}
	

	/*
		Populates the delta_time structure. 
		It takes time O(s), where s is the number
		of elements in delta_time.
	*/
	void create_delta_time(vector<double>& A)
	{
		delta_time.resize(A.size() + 1);
		for (int i = 0; i < A.size() + 1; ++i) delta_time[i].clear();
		rmq_delta_time.resize(A.size() + 1);

		construct(0, (int) A.size() - 1);

		rmq_top.preprocess(top);

		for (int i = 0; i < delta_time.size(); ++i){
			vector<double> r(delta_time[i].size());
			for (int j = 0; j < r.size(); ++j) r[j] = delta_time[i][j].first;
			rmq_delta_time[i].preprocess(r);

		}
	}
	

public:
	RMQHeuristicCount(){
		// constructor 
	}

	~RMQHeuristicCount(){
		// destructor
	}
	
	int get_presize(){
		int size = 0;
		for (int i = 0; i < delta_time.size(); ++i)
			size += delta_time[i].size();
		return size;
	}

	void preprocess(vector<double>& A){
		seq = A;

		rmq_max.preprocess(seq);
		rmq_min.preprocess(seq);

		top.resize(seq.size() + 1);
		fill(top.begin(), top.end(), 0);
	
		in_queue_s.resize(seq.size());
		in_queue_e.resize(seq.size());
		
		hash_s.resize(seq.size());
		hash_e.resize(seq.size());

		nquery = 0;
		fill(in_queue_s.begin(), in_queue_s.end(), nquery);
		fill(in_queue_e.begin(), in_queue_e.end(), nquery);
		fill(hash_s.begin(), hash_s.end(), nquery);
		fill(hash_e.begin(), hash_e.end(), nquery);

		create_delta_time(seq);
	}

	/* 
		Calculate all solutions with start f. 
	*/
	void findSolutionsByStart(int f, int s, int e, int t, int d){
		if (s > e) return;
		stack<pair<int, int> >* Q = new stack<pair<int, int> >();
		Q->push(make_pair(s, e));
		while (!Q->empty()){
			pair<int, int> now = Q->top(); Q->pop();
			s = now.first; e = now.second;
			if (s > e) continue;
			int m = rmq_max.query(s, e);
			if (seq[m] - seq[f] < d) continue;
			assert (m > f);
			if (hash_e[m] != nquery)
				++ans;
			if (in_queue_e[m] != nquery) {
					in_queue_e[m] = nquery;
					QStartEnd->push(make_pair(m, 'E'));
			}
			Q->push(make_pair(m + 1, e));
			Q->push(make_pair(s, m - 1));
		}
		delete Q;
	}

	/*
		Calculate all solutions with end f	
	*/
	void findSolutionsByEnd(int f, int s, int e, int t, int d){
		if (s > e) return;
		stack<pair<int, int> >* Q = new stack<pair<int, int> >();
		Q->push(make_pair(s, e));

		while (!Q->empty()){
			pair<int, int> now = Q->top(); Q->pop();
			s = now.first; e = now.second;
			if (s > e) continue;
			int m = rmq_min.query(s, e);
			assert (f > m);
			if (seq[f] - seq[m] < d) continue;
			if (hash_s[m] != nquery)
				++ans;
			if (in_queue_s[m] != nquery) {
					in_queue_s[m] = nquery;
					QStartEnd->push(make_pair(m, 'S'));
			}
			Q->push(make_pair(m + 1, e));
			Q->push(make_pair(s, m - 1));
		}
		delete Q;
	}


	/*
		Discover all starts and ends of solutions  
		with time difference k inside delta_time[k][s] ... delta_time[k][e].
	*/
	
	void discoverStartEnd(int k, int s, int e, int t, int d){
		QStartEnd = new stack<pair<int, char> >();

		// this is stack is used to simulate recursion
		stack<pair<int, int> >* Q = new stack<pair<int, int> >();
		Q->push(make_pair(s, e));
		
		while (!Q->empty()){
			pair<int, int> now = Q->top(); Q->pop();
			s = now.first; e = now.second;
			if (s > e) continue;
			int m = rmq_delta_time[k].query(s, e);
			if (delta_time[k][m].first < d) continue;

			int new_s = delta_time[k][m].second;
			int new_e = min(new_s + t, (int) seq.size() - 1);

			if (in_queue_s[new_s] != nquery){
				in_queue_s[new_s] = nquery;
				QStartEnd->push(make_pair(new_s, 'S'));
			}
			if (in_queue_e[new_e] != nquery){
				in_queue_e[new_e] = nquery;
				QStartEnd->push(make_pair(new_e, 'E'));
			}
	
			Q->push(make_pair(s, m - 1));
			Q->push(make_pair(m + 1, e));
		}
		
		while (!QStartEnd->empty()){
			pair<int, char> now = QStartEnd->top(); QStartEnd->pop();
			if (now.second == 'S'){
				int new_s = now.first;
				int new_e = min(new_s + t, (int) seq.size() - 1);
				findSolutionsByStart(new_s, new_s + 1, new_e, t, d);
				hash_s[new_s] = nquery;
			}

			if (now.second == 'E'){
				int new_e = now.first;
				int new_s = max(new_e - t, 0);
				findSolutionsByEnd(new_e, new_s, new_e - 1, t, d);
				hash_e[new_e] = nquery;
			}
		 }

		delete Q;
		delete QStartEnd;
	}
	

	/* 
		For all k in range [s, e], finds 
		the k's with at least one
		solution to the query (t, d). 
		(that is, find times k \in [s, e] such as there is
		at least one interval of size k which is a solution 
		for the query (t, d))
	*/
	void findTimesWithSolution(int s, int e, int t, int d){
		stack<pair<int, int> >* Q = new stack<pair<int, int> >();
		Q->push(make_pair(s, e));
		
		while (!Q->empty()){
			pair<int, int> now = Q->top(); Q->pop();
			s = now.first; e = now.second;
			if (s > e) continue;
			int m = rmq_top.query(s, e);
			if (top[m] < d) continue;
			// there is at least one solution in list m 
			discoverStartEnd(m, 0, delta_time[m].size() - 1, t, d);

			Q->push(make_pair(s, m - 1));
			Q->push(make_pair(m + 1, e));
		}
		delete Q;
	}

	long long query(int t, double d){
		assert (1<= t && t < delta_time.size());
		ans = 0;
		++nquery;
		findTimesWithSolution(1, t, t, d);
		return ans;
	}
};
