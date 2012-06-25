#ifndef __BEG__NAIVE_SCAN_
#define __BEG__NAIVE_SCAN_

#include <assert.h> // for assert 

#include "../heuristic.cpp" // for the abstract heuristic class

using namespace std;

template<class T>
class NaiveScan : public Heuristic<T>{
	// a copy of the input sequence
	vector<T> seq;

	// count how many answers were found for the query
	long long ans;
	
public:
	NaiveScan(){

	}
	~NaiveScan(){
	}

	// see the abstract heuristic class for
	// a definition of this function
	int get_presize(){
		return 42;
	}

	string get_name(){
		return "NaiveScan";
	}

	// this heuristic preprocessing is
	// simple, since it only needs a RMQ structure
	void preprocess(vector<T>& A)
	{
		seq = A;
	}

	long long query(int delta_t, T delta_v){
		// only positive deltas
		int t = delta_t;
		T d = delta_v;
		ans = 0;
		int n = seq.size();
		// this loop fix each possible beginning 
		// and checks inside it if it is valid
		for (int i = 0; i < n; ++i){
			for (int k = i + 1; k <= i + t && k < n; ++k)
				if (seq[k] - seq[i] >= d) {
					++ans;
					break;
				}
		}
		return ans;
	}
};
#endif
