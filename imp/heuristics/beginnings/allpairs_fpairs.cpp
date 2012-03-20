#ifndef __BEG_ALLPAIRS_FPAIRS__
#define __BEG_ALLPAIRS_FPAIRS__

#include <iostream>
#include <vector>
#include <algorithm>
#include <assert.h>
#include "../../../../rmq/cpp/rmq_bucket.cpp"
#include "../heuristic.cpp"
#include "../allpairs/h_fpairs.cpp"

using namespace std;

template<class T>
class AllPairsFPairs: public Heuristic<T>
{
	HFPairs<T> *h;
	int n;

public:
	AllPairsFPairs(){
		h = new HFPairs<T>();
	}
	
	~AllPairsFPairs(){
		delete h;
	}
	
	int get_presize(){
		return 42;
	}

	string get_name(){
		return "AllPairs_FPair for beginnings";
	}


	void preprocess(vector<T>& A){
		n = (int) A.size();
		h->preprocess(A);	
	}

	// <= t and >= d
	// the pair (a_i, a_j) has distance of j - i and size a_j - a_i
	long long query(int delta_t, T delta_v){
		vector<pair<int, int> > allpairs_answer = h->enumQuery(delta_t, delta_v);
//		cout<<"Allpais: "<<endl;
//		for (int i = 0; i < allpairs_answer.size(); ++i)
//			cout<<"("<<allpairs_answer[i].first<<", "<<allpairs_answer[i].second<<")"<<endl;
		vector<bool> hash(n, false);
		long long unique_inics = 0;
		for (int i = 0; i < allpairs_answer.size(); ++i){
			if (hash[allpairs_answer[i].first]) continue;
			++unique_inics;
			hash[allpairs_answer[i].first] = true;
//			cout<<allpairs_answer[i].first<<", ";
		}
//		cout<<endl;
		return unique_inics;	
	}

};
#endif
