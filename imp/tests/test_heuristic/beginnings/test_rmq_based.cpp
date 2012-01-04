
#include <cassert>
#include <iostream>
#include <algorithm>
#include <vector>

#include "../../../heuristics/beginnings/rmq_based.cpp"

using namespace std;

int calc_by_brute(vector<int>& A, int t, int d)
{
	int res = 0;
	for (int i = 0; i < A.size(); ++i){
		bool is_begin = false;
		for (int j = i; j < A.size() && j <= i + t; ++j)
				if (A[j] - A[i] >= d) {
					is_begin = true;
					break;
				}
		if (is_begin) ++res;
		}
	return res;
}

int main()
{
	RmqBased<int> h = RmqBased<int>();
	int n = 10;
	vector<int> A(n);
	for (int i = 0; i < n; ++i) A[i] = i + 1;

	for (int test = 0; test <= 100; ++test){
		random_shuffle(A.begin(), A.end());
		h.preprocess(A);
		for (int t = 1; t <= n; ++t)
			for (int d = 1; d <= n; ++d){
				int  got = h.query(t, d);
				int  expected = calc_by_brute(A, t, d);
				if (got != expected){
					cout<<"Something went wrong. Below the details:"<<endl;
					cout<<"(t = "<<t<<", "<<"d = "<<d<<")"<<endl;
					for (int k = 0; k < A.size(); ++k) cout<<A[k]<<", ";
					cout<<endl;
					assert(false);
				}
			}
			cout<<"test "<<test<<": "<<"SUCESSS!!!"<<endl;
	}
		 
	return 0;
}
