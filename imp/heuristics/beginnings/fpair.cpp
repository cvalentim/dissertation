#ifndef __FPAIR__
#define __FPAIR__ 

#include <vector>

#include "../../../../rmq/cpp/rmq_naive.cpp"

using namespace std;


struct FPair{
		int s, e;	
		double vs, ve;
		
		FPair(){} 

		FPair(int _s, int _e){
				s = _s;
				e = _e;
		}

		FPair(int _s, int _e, double _vs, double _ve){
			s = _s, e = _e, vs = _vs, ve = _ve;
		}

		int get_delta_t() const{
				return e - s;
		}
	
		double get_delta_v() const{
				return ve - vs;
		}
		
		bool operator==(const FPair& rhs) const{
				return s == rhs.s && e == rhs.e;
		}
		// less than operator ?
		
		friend ostream& operator<<(ostream& out, const FPair& x ) 
		{
			out << "(" << x.s << ", " << x.e << ", " << x.vs <<", "<< x.ve <<")"<<endl;
			return out;
		}
};

// FIXME: Make the input vector generic, possibly using
// templates
template<typename T>
void construct(int s, int e, vector<T>& A, RMQNaive<T>& rmq_max, RMQNaive<T>& rmq_min, vector<FPair>& res ){
		if (s > e) return;
		int hi = rmq_max.query(s, e, greater<T>());
		int lo = rmq_min.query(s, hi);
		while (lo < hi && A[lo] < A[hi]){
			res.push_back(FPair(lo, hi, A[lo], A[hi]));
			lo = rmq_min.query(lo + 1, hi);		
		}
		construct(s, hi - 1, A, rmq_max, rmq_min, res);
		construct(hi + 1, e, A, rmq_max, rmq_min, res);
}

template<typename T>
vector<FPair> find_fpairs(vector<T>& A){
	vector<FPair> res;
	RMQNaive<T> rmq_max, rmq_min;

	rmq_max.preprocess(A, greater<T>());
	rmq_min.preprocess(A);

	construct(0, A.size() - 1, A, rmq_max, rmq_min, res);

/*	for (int i = 0; i < A.size(); ++i){
		int smallest = i, greatest = i;
		for (int j = i + 1; j < A.size(); ++j){
				if (A[j] < A[smallest]) smallest = j;
				if (A[j] > A[greatest]) greatest = j;
				if (A[greatest] == A[j] && A[smallest] == A[i])
						 res.push_back(FPair(i, j, A[i], A[j]));
		}
	}*/
	return res;
}

#endif
