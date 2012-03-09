#include <vector>
#include <algorithm>
#include <cassert>
#include <cstdio>

#include "allpairs_fpairs.cpp"

using namespace std;

long long brute_force(const vector<int>& A, int t, int d){
		long long res = 0;
		for (int i = 0; i < A.size(); ++i)
			for (int j = i + 1; j < A.size() && j <= i + t; ++j){
					if (A[j] - A[i] >= d) {++res; break;}
			}
		return res;
}

int main(){
		int n = 13;
		int interations = 1000;
		vector<int> A(n);
		AllPairsFPairs<int> h;

		for (int i = 0; i < n; ++i) A[i] = i;
		for (int i = 0; i < interations; ++i){
				printf("%d\n", i);
				random_shuffle(A.begin(), A.end());
				h.preprocess(A);
				for (int t = 2; t <= n; ++t)
					for (int d = 2; d <= n; ++d){
							long long got = h.query(t, d);
							long long exp = brute_force(A, t, d);
							if (got != exp){
								printf("got = %lld exp = %lld\n", got, exp);
								printf("t = %d d = %d\n", t, d);
								for (int i = 0; i < n; ++i) 
									printf("%d,  ", A[i]);
								printf("\n");		
								assert (got == exp);
							}
					}	
		}
		cout<<"Successful!"<<endl;
}
