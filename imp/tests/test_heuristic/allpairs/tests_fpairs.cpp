#include "../../heuristics/fpairs.cpp"
#include <assert.h>
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

vector<pair<int, int> > calc_by_brute(vector<double>& A, int t, int d)
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
	FPairs<double> h;
	vector<double> A;	
	for (int n = 2; n <= 100; ++n){
			A.resize(n);
			for (int i = 0; i < n; ++i) A[i] = i + 1;
			for (int test = 0; test <= 50; ++test){
				random_shuffle(A.begin(), A.end());
				h.preprocess(A);
				for (int t = 1; t <= n; ++t)
					for (int d = 1; d <= n; ++d){
						int got = h.query(t, d);
						vector<pair<int, int> > exp = calc_by_brute(A, t, d);
						if (got != exp.size()){
							for (int k = 0; k < A.size(); ++k)
								cout<<A[k]<<", ";
							cout<<endl;
							cout<<"t = "<<t<<" d = "<<d<<endl;
							cout<<"got: "<<got<<endl;
							//for (int k = 0; k < got.size(); ++k)
							//	cout<<"("<<got[k].first<<", "<<got[k].second<<") ";
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
