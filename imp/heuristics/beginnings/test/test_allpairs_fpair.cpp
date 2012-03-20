#include <vector>
#include <algorithm>
#include <cassert>
#include <cstdio>

#include "../rmq_based.cpp"
#include "../allpairs_fpairs.cpp"
#include "../../../../../rmq/cpp/rmq_bucket.cpp" 

using namespace std;

long long brute_force(const vector<int>& A, int t, int d){
		long long res = 0;
		for (int i = 0; i < A.size(); ++i){
			for (int j = i + 1; j < A.size() && j <= i + t; ++j){
					if (A[j] - A[i] >= d) {
						++res; 
						break;
					 }
			}
		}
		return res;
}

int main(){
		int n = 50;
		int interations = 1000;
		vector<int> A(n);
		AllPairsFPairs<int> h;
		RmqBased<int> h2(new RMQBucket<int>(RMQTypes_t::MAX()));

		
		vector<double> B(5);
		B[0] = 2.0, B[1] = 1.0, B[2] = 1.2345, B[3] = 9.3, B[4] = 2.5;
		AllPairsFPairs<double> h_d;
		RmqBased<double> h2_d(new RMQBucket<double>(RMQTypes_t::MAX()));
		h_d.preprocess(B);
		h2_d.preprocess(B);
		assert (h_d.query(1, 2) == h2_d.query(1, 2));
		assert (h_d.query(1, 1.5) == h2_d.query(1, 1.5));
		assert (h_d.query(1, 3.2) == h2_d.query(1, 3.2));
		assert (h_d.query(1, 4.5) == h2_d.query(1, 4.5));

		assert (h_d.query(2, 1) == h2_d.query(2, 1));
		assert (h_d.query(2, 10.494) == h2_d.query(2, 10.494));
		assert (h_d.query(2, 5.5) == h2_d.query(2, 5.5));
		int a = h_d.query(2, 0.5);
		int b = h2_d.query(2, 0.5);
		assert (h_d.query(2, 0.5) == h2_d.query(2, 0.5));

		assert (h_d.query(3, 2.5) == h2_d.query(3, 2.5));
		assert (h_d.query(4, 3.5) == h2_d.query(4, 3.5));


		for (int i = 0; i < n; ++i) A[i] = i;
		for (int i = 0; i < interations; ++i){
				printf("interation = %d\n", i);
				random_shuffle(A.begin(), A.end());
				h.preprocess(A);
				h2.preprocess(A);
				for (int t = 2; t <= n; ++t)
					for (int d = 2; d <= n; ++d){
							long long got = h.query(t, d);
							long long exp = h2.query(t, d);
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
