#ifndef __ALLPAIRS_FPAIRS__
#define __ALLPAIRS_FPAIRS__

#include <iostream>
#include <algorithm>
#include <assert.h>
#include "../../../../rmq/cpp/rmq_bucket.cpp"
#include "../heuristic.cpp"
#include "../beginnings/range_list/fastSpecialPairs.cpp"

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
	vector<vector<SpecialPair<T> > > delta_time;
	vector<SpecialPair<T> > specialPairs;

	RMQBucket<SpecialPair<T> > *rmq_delta_time;
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

    // number of solutions found
    int nans;

	void construct(int s, int e){
		if (s > e) return;
		int hi = rmq_max->query(s, e);
		int lo = rmq_min->query(s, hi);
		while (lo < hi && seq[lo] < seq[hi]){
			delta_time[hi - lo].push_back(SpecialPair<T>(seq[hi] - seq[lo], lo, hi));
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
		//construct(0, (int) seq.size() - 1);
		vector<SpecialPair<T> > SP;
		genSpecialPairs(seq, SP); 
		for (int i = 0; i < SP.size(); ++i)
			delta_time[SP[i].getDelta()].push_back(SP[i]);
		howMany.resize(seq.size() + 1);
		specialPairs.clear();
		howMany[0] = 0;
		assert(delta_time[0].size() == 0);
		for (int i = 1; i < delta_time.size(); ++i){
			howMany[i] = delta_time[i].size() + howMany[i - 1];
			for (int j = 0; j < delta_time[i].size(); ++j)
						specialPairs.push_back(delta_time[i][j]);
		}
		rmq_delta_time->preprocess(specialPairs);
		delta_time.clear();
	}

	void create_delta_time2(){
		vector<SpecialPair<T> > Sp;
		genSpecialPairs(seq, Sp);
		vector<int> amount(seq.size() + 1, 0);
		for (int i = 0; i < Sp.size(); ++i)
			amount[Sp[i].getDelta()]++;
		vector<int> startAt(seq.size() + 1);
		howMany.resize(seq.size() + 1);
		startAt[0] = 0;
		howMany[0] = 0;
		for (int i = 1; i < seq.size() + 1; ++i){
			howMany[i] = howMany[i - 1] + amount[i];
			startAt[i] = howMany[i - 1];
		}
		specialPairs.resize(Sp.size());
		for (int i = 0; i < Sp.size(); ++i){
			int t = Sp[i].getDelta();
			specialPairs[startAt[t]] = Sp[i];
			++startAt[t];
		}
		rmq_delta_time->preprocess(specialPairs);
	}

 	// closed interval [s, e]
	void findStart0(int s, int e, int t, T d){
		if (s > e) return;
		int m = rmq_delta_time->query(s, e);	
		if (specialPairs[m].getDesviation() < d) return;
		if (in_start0[specialPairs[m].s] != nquery){
				start0.push_back(specialPairs[m].s);
				in_start0[start0.back()] = nquery;
		}
		findStart0(s, m - 1, t, d);
		findStart0(m + 1, e, t, d);
	}

    // Find all ends of speciais (t, d)-events
	void _genSpecialEnds(int s, int e, int t, T d, vector<int>& se, vector<bool>& hash){
		if (s > e) return;
		int m = rmq_delta_time->query(s, e);	
		if (specialPairs[m].getDesviation() < d) return;
		if (hash[specialPairs[m].e] != nquery){
				se.push_back(specialPairs[m].e);
				hash[se.back()] = nquery;
		}
		_genSpecialEnds(s, m - 1, t, d, se, hash);
		_genSpecialEnds(m + 1, e, t, d, se, hash);
	}

	
	// closed interval [s, e]
    // In paper: GenEventsStart
	void genEventsStart(int f, int s, int e, T d){
			if (s > e) return;
			int m = rmq_max->query(s, e);
			if (seq[m] - seq[f] < d) return;
			if (in_end0[m] != nquery){
					end0.push_back(m);
					in_end0[m] = nquery;
			}
			//ans.push_back(make_pair(f, m));
            ++nans;
			genEventsStart(f, s, m - 1, d);
			genEventsStart(f, m + 1, e, d);
	}

	// closed interval [s, e]
    // In paper: GenEventsEnd
	void genEventsEnd(int f, int s, int e, T d){
			if (s > e) return;
			int m = rmq_min->query(s, e);
			if (seq[f] - seq[m] < d) return;
			if (in_start0[m] != nquery){
					start1.push_back(m);
				  	//ans.push_back(make_pair(m, f));
                    ++nans;
			}
			genEventsEnd(f, s, m - 1, d);
			genEventsEnd(f, m + 1, e, d);
	}

	void solve(int t, T d){
		start0.clear(); end0.clear();
		findStart0(0, howMany[t] - 1, t, d);
		for (int i = 0; i < start0.size(); ++i){
				int e = min(start0[i] + t, (int)seq.size() - 1);
				genEventsStart(start0[i], start0[i] + 1, e, d);
		}
		for (int i = 0; i < end0.size(); ++i){
				int s = max(end0[i] - t, 0);
				genEventsEnd(end0[i], s, end0[i] - 1, d);
		}
	}

   

public:
	HFPairs(){
		// constructor 
		rmq_delta_time = new RMQBucket<SpecialPair<T> >(RMQTypes_t::MAX());
		rmq_max = new RMQBucket<T>(RMQTypes_t::MAX());
		rmq_min = new RMQBucket<T>(RMQTypes_t::MIN());
	}

	~HFPairs(){
		// destructor
		delete rmq_max;
		delete rmq_min;
		delete rmq_delta_time;
	}


	vector<int> genSpecialEnds(int t, T d){
        vector<bool> hash(seq.size() + 1);
        fill(hash.begin(), hash.end(), nquery);
        vector<int> se; // special ends
        ++nquery;
	    _genSpecialEnds(0, howMany[t] - 1, t, d, se, hash);
        return se;
    }

	int get_presize(){
		return specialPairs.size();
	}

	int amount_fpair(){
			return howMany[howMany.size() - 1];
	}
	
	string get_name(){
			return  "AllPairs-SP";
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
		create_delta_time2();
	}

	vector<pair<int, int> > enumQuery(int t, T d){
		assert (1<= t && t < seq.size() + 1);
		++nquery;
		ans.clear();
        nans = 0;
		solve(t, d);
        //sort(ans.begin(), ans.end());
        //for (int i = 1; i < ans.size(); ++i)
        //      assert(ans[i] != ans[i -1]);
		return ans;
	
	}

	// <= t and >= d
	// the pair (a_i, a_j) has distance of j - i and size a_j - a_i
	long long query(int t, T d){
        int found_ans = (long long) enumQuery(t, d).size();
       // assert (found_ans == nans);
        return nans;
	}
};
#endif
