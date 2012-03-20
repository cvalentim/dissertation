#include <assert.h>
#include <iostream>
#include <algorithm>

#include "../pure_rmq.cpp"
#include "../../../../../rmq/cpp/rmq_bucket.cpp"
#include "../../../../../rmq/cpp/rmq_st.cpp"


using namespace std;

int calc_by_brute(vector<int>& A, int t, int d)
{
	int res = 0;
	for (int i = 0; i < A.size(); ++i)
		for (int j = i + 1; j < min((int)A.size(), i + t + 1); ++j){
			if (j - i > t) break;
			if (A[j] - A[i] >= d) ++res;
		}
	return res;
}

int main()
{
	PureRmq<int> h(new RMQBucket<int>(RMQTypes_t::MAX()));
	vector<int> A;
	int n = 10;
	A.resize(10);
	for (int i = 0; i < n; ++i) A[i] = i + 1;
	for (int test = 0; test <= 100000; ++test){
		random_shuffle(A.begin(), A.end());
		h.preprocess(A);
		for (int t = 1; t <= n; ++t)
			for (int d = 1; d <= n; ++d){
				int  got = h.query(t, d);
				int exp = calc_by_brute(A, t, d);
				if (got != exp){
					for (int k = 0; k < A.size(); ++k)
						cout<<A[k]<<", ";
					cout<<endl;
					cout<<"t = "<<t<<" d = "<<d<<endl;
					cout<<endl;
					cout<<"============================================"<<endl;
					cout<<endl;
					assert(false);
				}
			}
			cout<<"test "<<test<<": "<<"SUCESSS!!!"<<endl;
	}
		 
	return 0;
}
