#include "sort_heuristic.cpp"
#include <assert.h>
#include <iostream>
#include <algorithm>

using namespace std;

long long calc_by_brute(vector<int>& A, int t, int d)
{
	long long res = 0;
	for (int i = 0; i < A.size(); ++i)
		for (int j = i + 1; j < A.size(); ++j){
			if (j - i > t) break;
			if (A[j] - A[i] >= d) ++res;
		}
	return res;
}

int main()
{
	SortHeuristic h = SortHeuristic();
	vector<int> A;

	
	for (int n = 2; n <= 15; ++n){
			A.resize(n);
			for (int i = 0; i < n; ++i) A[i] = i + 1;
			for (int test = 0; test <= 50; ++test){
				random_shuffle(A.begin(), A.end());
				h.preprocess(A);
				for (int t = 1; t <= n; ++t)
					for (int d = 1; d <= n; ++d){
						long long got = h.query(t, d);
						long long exp = calc_by_brute(A, t, d);
						if (got != exp){
							for (int k = 0; k < A.size(); ++k)
								cout<<A[k]<<", ";
							cout<<endl;
							cout<<"t = "<<t<<" d = "<<d<<endl;
							cout<<"got: "<<got<<endl;
							cout<<"exp: "<<exp<<endl;
							cout<<endl;
							cout<<endl;
							assert(false);
						}
					}
					cout<<"test "<<test<<": "<<"SUCESSS!!!"<<endl;
			}
	}
	return 0;
}
