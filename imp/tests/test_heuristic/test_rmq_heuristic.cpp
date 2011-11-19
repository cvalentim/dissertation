#include "rmq_heuristic.cpp"
#include <assert.h>
#include <iostream>
using namespace std;

int main()
{
	/*RMQHeuristic h = RMQHeuristic();
	int AA[8] = { 3, 4, 6, 2, 9, 7, 1, 8};
	vector<int> A(AA, AA + 8);
	h.preprocess(A);
	for (int i = 0 ; i < 9; ++i){
		cout<<"i = "<<i<<endl;
		for (int t = 0; t < h.delta_time[i].size(); ++t){
			int fi = h.delta_time[i][t].first;
			int se = h.delta_time[i][t].second;
			cout<<"("<<fi<<", "<<se<<")"<<" ";
		}
		cout<<endl;
	}*/
		
	RMQHeuristic h2 = RMQHeuristic();
	vector<int> A2;
	A2.resize(100);
	for (int i = 0; i < 100; ++i) A2[i] = i + 1;
	h2.preprocess(A2);
	cout<<"presize = "<<h2.get_presize()<<endl;
	assert(h2.get_presize() ==  (100 * 99)/2);
	return 0;
}
