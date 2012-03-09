#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>

#include "../rmq_based.cpp"
#include "../../../../../rmq/cpp/rmq_bucket.cpp" 
#include "../../../../../rmq/cpp/rmq_st.cpp" 

using namespace std;

long long brute_force(const vector<int>& A, int t, int d){
		long long res = 0;
		for (int i = 0; i < A.size(); ++i){
			for (int j = i + 1; j < A.size() && j <= i + t; ++j){
					if (A[j] - A[i] >= d) {++res; break;}
			}
		}
		return res;
}

bool do_test1(){
	int n = 13;
	int interations = 1000;
	vector<int> A(n);
	RmqBased<int> h(new RMQBucket<int>(RMQTypes_t::MAX()));
	for (int i = 0; i < n; ++i) A[i] = i;

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
	return true;
}

bool do_test2(){
	int n = 13;
	int interations = 1000;
	vector<int> A(n);
	RmqBased<int> h(new RMQSt<int>(RMQTypes_t::MAX()));
	for (int i = 0; i < n; ++i) A[i] = i;

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
	return true;
}



int main(){
	if (do_test1()) 
		cout<<"Success for RMQBucket!!!"<<endl;	
	if (do_test2()) 
		cout<<"Success for RMQSt!!!"<<endl;	
}
