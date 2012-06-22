#ifndef __BEG_ALLPAIRS_FPAIRS__
#define __BEG_ALLPAIRS_FPAIRS__

#include <iostream>
#include <vector>
#include <algorithm>
#include <assert.h>
#include "range_list/range_list.cpp"

#include "../../../../rmq/cpp/rmq_bucket.cpp"
#include "../heuristic.cpp"
//#include "../allpairs/h_fpairs.cpp"
#include "../allpairs/specialPairsH.cpp"


using namespace std;

template<class T>
class AllPairsFPairs: public Heuristic<T>
{
	HFPairs<T> *h_all;
    RangeList<T> *h_beg;
	int n;

public:
	AllPairsFPairs(){
		h_all = new HFPairs<T>();
        h_beg = new RangeList<T>();
	}
	
	~AllPairsFPairs(){
		delete h_all;
        delete h_beg;
	}
	
	int get_presize(){
		return 42;
	}

	string get_name(){
		return "Beg-Hybrid";
	}


	void preprocess(vector<T>& A){
		n = (int) A.size();
		h_all->preprocess(A);	
        h_beg->light_preprocess(A);
	}

	// <= t and >= d
	// the pair (a_i, a_j) has distance of j - i and size a_j - a_i
	long long query(int delta_t, T delta_v){
         vector<int> ends = h_all->genSpecialEnds(delta_t, delta_v);
         return h_beg->BegByEnd(ends, delta_t, delta_v).size();
	}

};
#endif
