#include <iostream>
#include <assert.h>
#include <vector>
#include <algorithm>

using namespace std;

class SortHeuristic
{
	vector<double> seq;
	vector<vector<pair<double, int> > > all_pairs;

	long long ans;
	
	public:
	SortHeuristic(){
	
	}
	
	int get_presize(){
		int res = 0;
		for (int i = 0; i < all_pairs.size(); ++i)
			res += all_pairs[i].size();
		return res;
	}

	void preprocess(vector<double>& A)
	{
		seq = A;
		all_pairs.resize(A.size() + 1);
		for (int i = 0; i < all_pairs.size(); ++i) all_pairs[i].clear();
		for (int i = 0; i < seq.size(); ++i)
			for (int j = i + 1; j < seq.size(); ++j) if (seq[j] > seq[i]){
				int t = j - i;
				all_pairs[t].push_back(make_pair(seq[j] - seq[i], i));
			}
		for (int i = 0; i < all_pairs.size(); ++i){
			sort(all_pairs[i].begin(), all_pairs[i].end());
			// bigger first
			reverse(all_pairs[i].begin(), all_pairs[i].end());
		}
		
	}

	long long query(int t, int d){
		assert (t > 0);
		ans = 0;
		for (int i = 1; i <= t && i < all_pairs.size(); ++i){
			int j = 0;
			while (j < all_pairs[i].size() && all_pairs[i][j].first >= d){
				++ans;
				++j;
			}

		}
		return ans;
	}
};
