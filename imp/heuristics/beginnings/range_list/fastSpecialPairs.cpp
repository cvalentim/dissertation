#ifndef __FAST_SPECIAL_PAIRS_
#define __FAST_SPECIAL_PAIRS_

#include <vector>
#include <iostream>

using namespace std;

template<class T>
struct SpecialPair{
	int s, e;	
	T vs, ve;

	SpecialPair(){} 

	SpecialPair(int _s, int _e){
		s = _s;
		e = _e;
	}

	SpecialPair(int _s, int _e, T _vs, T _ve){
		s = _s, e = _e, vs = _vs, ve = _ve;
	}

	int getDelta() const{
		return e - s;
	}

	T getDesviation() const{
		return ve - vs;
	}

	bool operator==(const SpecialPair<T>& rhs) const{
		return s == rhs.s && e == rhs.e;
	}

	friend ostream& operator<<(ostream& out, const SpecialPair<T>& x ) 
	{
		out << "(" << x.s << ", " << x.e << ", " << x.vs <<", "<< x.ve <<")"<<endl;
		return out;
	}
};

template<typename T>
void genSpecialPairs(vector<T>& A, vector<SpecialPair<T> >& res){
	int n = A.size();
	vector<int> startCandidate(n);
	vector<T> biggestEnd(n, -1);
	res.reserve(n);
	int k = -1;
	for (int i = 0; i < n; ++i){
		while (k > -1 && A[i] <= A[startCandidate[k]]) --k;
		int j = k;
		while (j > -1 && biggestEnd[j] < A[i]){
			res.push_back(SpecialPair<T>(startCandidate[j], i, A[startCandidate[j]], A[i]));
			biggestEnd[j] = A[i];
			--j;
		}
		++k;
		startCandidate[k] = i;
		biggestEnd[k] = A[i];
	}
}

template<typename T>
int amSpecialPairs(vector<T>& A){
	vector<SpecialPair<T> > am;
	genSpecialPairs(A, am);
	return (int) am.size();
}
#endif
