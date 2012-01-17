#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <cstdlib>

#include "range_list.cpp"

using namespace std;

template<class T>
void printVector(vector<T> A){
	for (typename vector<T>::iterator it = A.begin(); it != A.end(); ++it)
		cout<<*it<<", ";
	cout<<endl;
}

void test_findByEnd(){
	RangeList<int> rl;
	vector<int> A(4);
	A[0] = 1, A[1] = 2, A[2] = 3, A[3] = 4;
	rl.preprocess(A);
	vector<int> got;
	rl.findByEnd(3, 0, 3, got, 2);
	vector<int> exp(2);
	exp[0] = 0, exp[1] = 1;
	assert (got == exp);
	got.clear();
	rl.findByEnd(3, 0, 3, got, 1);
	exp.push_back(2);
	assert (got == exp);	
	A.resize(10);
	for (vector<int>::iterator it = A.begin(); it != A.end(); ++it)
			*it = it - A.begin() + 1;
	for (int test = 0; test < 100; ++test){
			random_shuffle(A.begin(), A.end());
			rl.preprocess(A);
			for (int i = 0; i < A.size(); ++i)
				for (int j = 0; j < A.size(); ++j)
					for (int e = j; e < A.size(); ++e){
							for (int d = 0; d <= 10; ++d){
									vector<int> exp;
									for (int k = i; k < j; ++k) if (A[e] - A[k] >= d)
											exp.push_back(k);
									sort(exp.begin(), exp.end());
									got.clear();
									rl.findByEnd(e, i, j, got, d);
									sort(got.begin(), got.end());
									assert(got == exp);	
							}
					}

	}
	cout<<"FindByEnd: OK!"<<endl;
}

void test_BegByEnd(){
	RangeList<int> rl;
	vector<int> A(4); A[0] = 1, A[1] = 2, A[2] = 3, A[3] = 4;
	rl.preprocess(A);
	vector<int> E; E.push_back(1); E.push_back(3);
	printVector(rl.BegByEnd(E, 1, 0));
	printVector(rl.BegByEnd(E, 1, 2));
	printVector(rl.BegByEnd(E, 3, 1));

	A.resize(33);
	for (int i = 0; i < 10; ++i) A[i] = i + 1;
	for (int test = 0; test <= 10000; ++test){
			cout<<"test = "<<test<<endl;
			random_shuffle(A.begin(), A.end());
			rl.preprocess(A);
			E.clear();
			for (int e = 0; e < A.size(); ++e)
				if (rand()%2 == 1) E.push_back(e);
			for (int t = 1; t <= A.size(); ++t)
				for (int d = 2; d <= A.size(); ++d){
						vector<int> exp;
						for (int e = 0; e < E.size(); ++e)
							for (int k = max(0, E[e] - t); k < E[e]; ++k)
									if (A[E[e]] - A[k] >= d) exp.push_back(k);
						sort(exp.begin(), exp.end());
						exp.erase(unique(exp.begin(), exp.end()), exp.end());
						vector<int> got = rl.BegByEnd(E, t, d);
						sort(got.begin(), got.end());
						assert (got == exp);
				}
							
	}
	cout<<"BegByEnd: OK!"<<endl;
}


int main(){
	test_findByEnd();
	test_BegByEnd();
	return 0;
}
