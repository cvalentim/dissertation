#ifndef __FPAIR_HELPER_
#define __FPAIR_HELPER_

#include <vector>

#include "../../../../../rmq/cpp/rmq_bucket.cpp"

using namespace std;


template<class T>
struct FPair{
		int s, e;	
		T vs, ve;
		
		FPair(){} 

		FPair(int _s, int _e){
				s = _s;
				e = _e;
		}

		FPair(int _s, int _e, T _vs, T _ve){
			s = _s, e = _e, vs = _vs, ve = _ve;
		}

		int get_delta_t() const{
				return e - s;
		}
	
		T get_delta_v() const{
				return ve - vs;
		}
		
		bool operator==(const FPair<T>& rhs) const{
				return s == rhs.s && e == rhs.e;
		}
		// less than operator ?
		
		friend ostream& operator<<(ostream& out, const FPair<T>& x ) 
		{
			out << "(" << x.s << ", " << x.e << ", " << x.vs <<", "<< x.ve <<")"<<endl;
			return out;
		}
};

template<typename T>
void construct(int s, int e, vector<T>& A, RMQBucket<T>& rmq_max, RMQBucket<T>& rmq_min, vector<FPair<T> >& res ){
		if (s > e) return;
		int hi = rmq_max.query(s, e);
		int lo = rmq_min.query(s, hi);
		while (lo < hi && A[lo] < A[hi]){
			res.push_back(FPair<T>(lo, hi, A[lo], A[hi]));
			lo = rmq_min.query(lo + 1, hi);		
		}
		construct(s, hi - 1, A, rmq_max, rmq_min, res);
		construct(hi + 1, e, A, rmq_max, rmq_min, res);
}

template<typename T>
void find_fpairs(vector<T>& A, vector<FPair<T> >& res){
	RMQBucket<T> rmq_max(RMQTypes_t::MAX()), rmq_min(RMQTypes_t::MIN());

	rmq_max.preprocess(A);
	rmq_min.preprocess(A);

	construct(0, A.size() - 1, A, rmq_max, rmq_min, res);
}

#endif