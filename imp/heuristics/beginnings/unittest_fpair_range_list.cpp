#include "fpair_range_list.cpp"
#include "fpair.cpp"

void test_range_query_one_node(){
	RangeTreeFPair<int> rtree;
	vector<FPair<int> > A;
	A.push_back(FPair<int>(0, 1, 5, 7));

	rtree.range_preprocess(A);		
	assert(rtree.range_query(0, 2, 1).size() == 1);
	assert(rtree.range_query(0, 1, 1).size() == 1);
	assert(rtree.range_query(0, 1, 2).size() == 1);
	assert(rtree.range_query(0, 1, 3).size() == 0);
	assert(rtree.range_query(0, 0, 2).size() == 0);
	assert(rtree.range_query(1, 1, 2).size() == 1);
		
	cout<<"Range Query One Node: Successful!!!"<<endl;
}

void test_range_query_two_nodes(){
	RangeTreeFPair<int> rtree;
	vector<FPair<int> > A;

	A.push_back(FPair<int>(0, 1, 5, 7));
	A.push_back(FPair<int>(3, 7, 10, 15));

	rtree.range_preprocess(A);		
	assert(rtree.range_query(0, 2, 1).size() == 1);
	assert(rtree.range_query(0, 1, 1).size() == 1);
	assert(rtree.range_query(0, 1, 2).size() == 1);
	assert(rtree.range_query(0, 1, 3).size() == 0);
	assert(rtree.range_query(0, 0, 2).size() == 0);
	assert(rtree.range_query(1, 1, 2).size() == 1);
	assert(rtree.range_query(0, 10, 0).size() == 2);
	assert(rtree.range_query(1, 4, 2).size() == 2);
	assert(rtree.range_query(1, 4, 5).size() == 1);
		
	cout<<"Range Query Two Nodes: Successful!!!"<<endl;
}


void test_range_query_three_nodes(){
	RangeTreeFPair<int> rtree;
	vector<FPair<int> > A;

	A.push_back(FPair<int>(0, 1, 5, 7));
	A.push_back(FPair<int>(3, 7, 10, 15));
	A.push_back(FPair<int>(11, 18, 1, 15));

	rtree.range_preprocess(A);		
	assert(rtree.range_query(0, 2, 1).size() == 1);
	assert(rtree.range_query(0, 1, 1).size() == 1);
	assert(rtree.range_query(0, 1, 2).size() == 1);
	assert(rtree.range_query(0, 1, 3).size() == 0);
	assert(rtree.range_query(0, 0, 2).size() == 0);
	assert(rtree.range_query(1, 1, 2).size() == 1);
	assert(rtree.range_query(0, 10, 0).size() == 3);
	assert(rtree.range_query(1, 4, 2).size() == 2);
	assert(rtree.range_query(1, 4, 5).size() == 1);
	assert(rtree.range_query(1, 7, 2).size() == 3);
		
	cout<<"Range Query Three Nodes: Successful!!!"<<endl;
}

bool lt_lexicographic2(const FPair<int>& a, const FPair<int> &b){
		if (a.s != b.s) return a.s < b.s;
		return a.e < b.e;
}

int brute_force(const vector<FPair<int> >& A, int l0, int l1, double hi){
		vector<FPair<int> > res;
		for (int i = 0; i < A.size(); ++i)
			if (l0 <= A[i].get_delta_t() && A[i].get_delta_t() <= l1 && A[i].get_delta_v() >= hi)
				res.push_back(A[i]);
		sort(res.begin(), res.end(), lt_lexico<int>);
		res.erase(unique(res.begin(), res.end()), res.end());		
		return res.size();
}


void test_random(){
	RangeTreeFPair<int> rtree;
	for (int test = 1; test < 10000; ++test){
		cout<<"test = "<<test<<endl;
		vector<FPair<int> > A;
		int n = 10;
		for (int i = 0; i < n; ++i){
				int s = rand()%100;
				int e = s + rand()%50;
				double v_s = (double) (rand()%10000);
				double v_e = (double) (rand()%10000);
				A.push_back(FPair<int>(s, e, v_s, v_e));
		}
		rtree.range_preprocess(A);
		for (int l0 = 1; l0 < 100; ++l0)
			for (int l1 = l0; l1 < 100; ++l1)
				for (int d = 1; d < 100; ++d){
						//cout<<"rtree = "<<rtree.range_query(l0, l1, (double)d).size()<<endl;
						//cout<<"brute = "<<brute_force(A, l0, l1, d)<<endl;
						if (rtree.range_query(l0, l1, (double) d).size() != brute_force(A, l0, l1, (double) d)){
							cout<<"l0 = "<<l0<<" l1 = "<<l1<<" hi = "<<d<<endl;
							cout<<"A = {";
							for (int k = 0; k < A.size(); ++k) cout<<A[k]<<",  ";
							cout<<"}"<<endl;							
							cout<<"rtree = "<<rtree.range_query(l0, l1, (double)d).size()<<endl;
							cout<<"brute = "<<brute_force(A, l0, l1, d)<<endl;

						}
				}
	}
	cout<<"Random: Succesful!!!"<<endl;
}


void test_bad_case(){
	RangeTreeFPair<int> rtree;
	vector<FPair<int> > A;

	A.push_back(FPair<int>(0, 12, 0, 3574));
	A.push_back(FPair<int>(0, 13, 0, -183));
	A.push_back(FPair<int>(0, 32, 0, 2270));
	A.push_back(FPair<int>(0, 27, 0, -2058));
	A.push_back(FPair<int>(0, 9, 0, -5662));
	A.push_back(FPair<int>(0, 46, 0, 3510));
	A.push_back(FPair<int>(0, 32, 0, -749));
	A.push_back(FPair<int>(0, 46, 0, -247));
	A.push_back(FPair<int>(0, 39, 0, -4876));
	A.push_back(FPair<int>(0, 17, 0, -199));

	int l0 = 46, l1 = 99;
	double hi = 1;
	rtree.range_preprocess(A);
	int got = (rtree.range_query(l0, l1, hi)).size();
	int exp = brute_force(A, l0, l1, hi);
	assert (got == exp);
	cout<<"Bad case1: Successful!"<<endl;
}

void test_bad_case2(){
	RangeTreeFPair<int> rtree;
	vector<FPair<int> > A;
	int l0 = 46, l1 = 99;
	double  hi = 65;
	A.push_back(FPair<int>(23, 35, 2964, 6538)); //t = 12
  A.push_back(FPair<int>(32, 45, 3348, 3165)); //t = 13
  A.push_back(FPair<int>(7, 39, 58, 2328)); //t = 32
  A.push_back(FPair<int>(37, 64, 6433, 4375)); //t = 27
  A.push_back(FPair<int>(23, 32, 8990, 3328));//t = 9
  A.push_back(FPair<int>(11, 57, 2485, 5995)); // t = 46
  A.push_back(FPair<int>(72, 104, 4831, 4082));//t = 32
  A.push_back(FPair<int>(5, 51, 6675, 6428)); // t = 46
  A.push_back(FPair<int>(58, 97, 9319, 4443)); // t = 39
  A.push_back(FPair<int>(54, 71, 3960, 3761));// t = 17
	// sorted by t: 9, 12, 13, 17, 27, 32, 32,  39, 46, 46	
	rtree.range_preprocess(A);
	int got = rtree.range_query(l0, l1, hi).size();
	int exp = brute_force(A, l0, l1, hi);
	if (got != exp){
			cout<<"exp = "<<exp<<endl;
			cout<<"got = "<<got<<endl;
	}
	assert (got == exp);
	cout<<"Bad case2: Successful!"<<endl;
}
void test_bad_case3(){
	RangeTreeFPair<int> rtree;
	vector<FPair<int> > A;

	int l0 = 6, l1 = 66;
	double  hi = 97;
	A.push_back(FPair<int>(16, 26, 6734, 8236));
	A.push_back(FPair<int>(26, 49, 7581, 6234));
	A.push_back(FPair<int>(15, 58, 2402, 5979));
	A.push_back(FPair<int>(16, 26, 2467, 9877));
	A.push_back(FPair<int>(62, 90, 9776, 9327));
	A.push_back(FPair<int>(63, 103, 1840, 8898));
	A.push_back(FPair<int>(47, 78, 3667, 3634));
	A.push_back(FPair<int>(74, 119, 4224, 642));
	A.push_back(FPair<int>(58, 67, 5230, 3884));
	A.push_back(FPair<int>(34, 46, 118, 2949));
	rtree.range_preprocess(A);
	int got = rtree.range_query(l0, l1, hi).size();
	int exp = brute_force(A, l0, l1, hi);
	
	assert (got == exp);	
	cout<<"Bad case3: Successful!"<<endl;
}
int main(){
	test_range_query_one_node();
	test_range_query_two_nodes();
	test_range_query_three_nodes();
	test_random();
	test_bad_case();
	test_bad_case2();
	test_bad_case3();
	return 0;
}
