#include "rmq_heuristic_linear.cpp"
#include <assert.h>
#include <iostream>
#include <algorithm>

using namespace std;

vector<pair<int, int> > calc_by_brute(vector<int>& A, int t, int d)
{
	vector<pair<int, int> > res;
	for (int i = 0; i < A.size(); ++i)
		for (int j = i + 1; j < A.size(); ++j){
			if (j - i > t) break;
			if (A[j] - A[i] >= d) res.push_back(make_pair(i, j));
		}
	return res;
}

int main()
{
	RMQHeuristicLinear h = RMQHeuristicLinear();
	vector<int> A;

	
	for (int n = 10000; n <= 10010; ++n){
			A.resize(n);
			for (int i = 0; i < n; ++i) A[i] = i + 1;
			for (int test = 0; test <= 100000; ++test){
				random_shuffle(A.begin(), A.end());
				h.preprocess(A);
				for (int t = 1; t <= n; ++t)
					for (int d = 1; d <= n; ++d){
						vector<pair<int, int> > got = h.query(t, d);
						vector<pair<int, int> > exp = calc_by_brute(A, t, d);
						sort(got.begin(), got.end());
						sort(exp.begin(), exp.end());
						if (got != exp){
							for (int k = 0; k < A.size(); ++k)
								cout<<A[k]<<", ";
							cout<<endl;
							cout<<"t = "<<t<<" d = "<<d<<endl;
							cout<<"got: "<<endl;
							for (int k = 0; k < got.size(); ++k)
								cout<<"("<<got[k].first<<", "<<got[k].second<<") ";
							cout<<endl;
							cout<<"exp: "<<endl;
							for (int k = 0; k < exp.size(); ++k)
								cout<<"("<<exp[k].first<<", "<<exp[k].second<<") ";			
							cout<<endl;
							cout<<"============================================"<<endl;
							cout<<endl;
							assert(false);
						}
					}
					cout<<"test "<<test<<": "<<"SUCESSS!!!"<<endl;
			}
	}
	return 0;
}
