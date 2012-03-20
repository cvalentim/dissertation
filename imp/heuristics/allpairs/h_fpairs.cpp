#ifndef __ALLPAIRS_FPAIRS__
#define __ALLPAIRS_FPAIRS__

#include <iostream>
#include <algorithm>
#include <assert.h>
#include "../../../../rmq/cpp/rmq_bucket.cpp"
#include "../heuristic.cpp"

using namespace std;


template<class T>
class HFPairs: public Heuristic<T>{
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
	vector<vector<pair<T, int> > > delta_time;
	vector<pair<T, int> > delta_time2;

	RMQBucket<pair<T, int> > *rmq_delta_time;
	vector<int> howMany;

	// rmqs to construct efficiently the delta_time structure
	RMQBucket<T> *rmq_min;
	RMQBucket<T> *rmq_max;

	// the input sequence
	vector<T> seq; 

	// =====================================================
  //  Query Structures	
	// ===================================================
	
	// starts and ends from the structure
	vector<int> start0, end0; 

	// starts and ends found by those on the structure
	vector<int> start1, end1;


	// an id for the current query, it's useful to make hash without
	// cleaning the hash vector at each query
	long long nquery;

	// a hash to know which start or end has already been processed
	vector<int> in_start0, in_end0;	

	// solutions found
	vector<pair<int, int> > ans;

	void construct(int s, int e){
		if (s > e) return;
		int hi = rmq_max->query(s, e);
		int lo = rmq_min->query(s, hi);
		while (lo < hi && seq[lo] < seq[hi]){
			delta_time[hi - lo].push_back(make_pair(seq[hi] - seq[lo], lo));
			lo = rmq_min->query(lo + 1, hi);		
		}
		construct(s, hi - 1);
		construct(hi + 1, e);
	}

	/*
		Populates the delta_time structure. 
		It takes time O(s), where s is the number
		of elements in delta_time.
	*/
	void create_delta_time()
	{
		delta_time.resize(seq.size() + 1);
		construct(0, (int) seq.size() - 1);
		howMany.resize(seq.size() + 1);
		delta_time2.clear();
		howMany[0] = 0;
		assert(delta_time[0].size() == 0);
		for (int i = 1; i < delta_time.size(); ++i){
			howMany[i] = delta_time[i].size() + howMany[i - 1];
			for (int j = 0; j < delta_time[i].size(); ++j)
						delta_time2.push_back(delta_time[i][j]);
		}
		rmq_delta_time->preprocess(delta_time2);
		delta_time.clear();
	}

public:
	HFPairs(){
		// constructor 
		rmq_delta_time = new RMQBucket<pair<T, int> >(RMQTypes_t::MAX());
		rmq_max = new RMQBucket<T>(RMQTypes_t::MAX());
		rmq_min = new RMQBucket<T>(RMQTypes_t::MIN());
	}

	~HFPairs(){
		// destructor
		delete rmq_max;
		delete rmq_min;
		delete rmq_delta_time;
	}
	
	int get_presize(){
		return delta_time2.size();
	}

	int amount_fpair(){
			return howMany[howMany.size() - 1];
	}
	
	string get_name(){
			return  "F-pairs for allpairs problem with RMQBucket";
	}


	void preprocess(vector<T>& A){
		seq = A;
		rmq_max->preprocess(seq);
		rmq_min->preprocess(seq);
		in_start0.resize(seq.size());
		in_end0.resize(seq.size());

		nquery = 0;
		fill(in_start0.begin(), in_start0.end(), nquery);
		fill(in_end0.begin(), in_end0.end(), nquery);
		create_delta_time();
	}

	// closed interval [s, e]
	void findStart0(int s, int e, int t, T d){
		if (s > e) return;
		int m = rmq_delta_time->query(s, e);	
		if (delta_time2[m].first < d) return;
		if (in_start0[delta_time2[m].second] != nquery){
				start0.push_back(delta_time2[m].second);
				in_start0[start0.back()] = nquery;
		}
		findStart0(s, m - 1, t, d);
		findStart0(m + 1, e, t, d);
	}
	
	// closed interval [s, e]
	void findByStart(int f, int s, int e, T d){
			if (s > e) return;
			int m = rmq_max->query(s, e);
			if (seq[m] - seq[f] < d) return;
			if (in_end0[m] != nquery){
					end0.push_back(m);
					in_end0[m] = nquery;
			}
			ans.push_back(make_pair(f, m));
			findByStart(f, s, m - 1, d);
			findByStart(f, m + 1, e, d);
	}

	// closed interval [s, e]
	void findByEnd(int f, int s, int e, T d){
			if (s > e) return;
			int m = rmq_min->query(s, e);
			if (seq[f] - seq[m] < d) return;
			if (in_start0[m] != nquery){
					start1.push_back(m);
				  	ans.push_back(make_pair(m, f));
			}
			findByEnd(f, s, m - 1, d);
			findByEnd(f, m + 1, e, d);
	}

	void solve(int t, T d){
		start0.clear(); end0.clear();
		findStart0(0, howMany[t] - 1, t, d);
		for (int i = 0; i < start0.size(); ++i){
				int e = min(start0[i] + t, (int)seq.size() - 1);
				findByStart(start0[i], start0[i] + 1, e, d);
		}
		for (int i = 0; i < end0.size(); ++i){
				int s = max(end0[i] - t, 0);
				findByEnd(end0[i], s, end0[i] - 1, d);
		}
	}

	vector<pair<int, int> > enumQuery(int t, T d){
		assert (1<= t && t < seq.size() + 1);
		++nquery;
		ans.clear();
		solve(t, d);
        sort(ans.begin(), ans.end());
        for (int i = 1; i < ans.size(); ++i)
              assert(ans[i] != ans[i -1]);
		return ans;
	
	}

	// <= t and >= d
	// the pair (a_i, a_j) has distance of j - i and size a_j - a_i
	long long query(int t, T d){
		return (long long) enumQuery(t, d).size();
	}
};
#endif
