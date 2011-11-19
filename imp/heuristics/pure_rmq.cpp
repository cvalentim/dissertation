#include <iostream>
#include <assert.h>
#include "../../../rmq/cpp/rmq_naive.cpp"
#include "../../../rmq/cpp/rmq_st.cpp"

using namespace std;

template<class T>
class PureRmq
{
	RMQNaive<T> rmqMax;
	vector<T> seq;
	long long ans;
	
	public:
	PureRmq(){
	
	}
	//WARNING: fix me!
	int get_presize(){
		//return rmqMax.length();
		return 42;
	}

	void preprocess(vector<T>& A)
	{
		seq = A;
		rmqMax.preprocess(seq, greater<double>());
	}


	/* all pairs which matching the index f in the
		closed interval [s, e] */
	void calculate(int f, int s, int e, int d)
	{
		//cout<<"f = "<<f<<"s = "<<s<<"e = "<<e<<"d = "<<d<<endl;
		if (s > e) return;

		assert(0 <= f && f < seq.size());
		assert(0 <= s && s < seq.size());
		assert(0 <= e && e < seq.size());


		int m = rmqMax.query(s, e, greater<double>());

		if (seq[m] - seq[f] < d) return;

		++ans;
		calculate(f, s, m - 1, d);
		calculate(f, m + 1, e, d); 
	}

	long long query(int t, int d){
		assert (t > 0);
		ans = 0;
		for (int i = 0; i < seq.size(); ++i){
			calculate(i, i, min(i + t, (int)seq.size() - 1), d);
		}
		return ans;
	}
};
