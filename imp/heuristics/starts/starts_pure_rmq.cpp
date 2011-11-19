#include <iostream>
#include <assert.h>
#include "../../rmq/rmq_st.cpp"

using namespace std;

class SPureRmq
{
	RMQStMax rmq;
	vector<double> seq;
	long long ans;
	
	public:
	SPureRmq(){
	
	}
	
	int get_presize(){
		return rmq.length();
	}

	void preprocess(vector<double>& A)
	{
		seq = A;
		rmq.preprocess(seq);
	}


	/* true if there is a pair matching the index f in the
		closed interval [s, e] */
	bool calculate(int f, int s, int e, int d)
	{
		//cout<<"f = "<<f<<"s = "<<s<<"e = "<<e<<"d = "<<d<<endl;
		if (s > e) return false;

		assert(0 <= f && f < seq.size());
		assert(0 <= s && s < seq.size());
		assert(0 <= e && e < seq.size());
		int m = rmq.query(s, e);
		if (seq[m] - seq[f] < d) return false;
		return true;
	}

	long long query(int t, int d){
		assert (t > 0);
		ans = 0;
		for (int i = 0; i < seq.size(); ++i){
			if (calculate(i, i, min(i + t, (int)seq.size() - 1), d))
				++ans;
		}
		return ans;
	}
};
