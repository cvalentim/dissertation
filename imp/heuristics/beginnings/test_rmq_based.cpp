#include <vector>
#include <algorithm>
#include <cassert>

#include "rmq_based.cpp"

using namespace std;

long long brute_force(const vector<int>& A, int t, int d){
		long long res = 0;
		for (int i = 0; i < A.size(); ++i)
			for (int j = i + 1; j < A.size() && j <= i + t; ++j){
					if (A[j] - A[i] >= d) ++res;
			}
		return res;
}

int main(){
		int n = 13;
		int interations = 1000;
		vector<int> A(n);
		RmqBased<int> h;

		for (int i = 0; i < interations; ++i){
				random_shuffle(A.begin(), A.end());
				h.preprocess(A);
				for (int t = 2; t <= n; ++t)
					for (int d = 2; d <= n; ++d){
							long long got = h.query(t, d);
							long long exp = brute_force(A, t, d);
							assert (got == exp);
					}	
		}
		cout<<"Successful!"<<endl;

}
