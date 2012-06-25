#include <vector>
#include <algorithm>
#include <cassert>

#include "../range_list.cpp"

using namespace std;

long long brute_force(const vector<int>& A, int t, int d){
		long long res = 0;
		for (int i = 0; i < A.size(); ++i){
			bool ok = false;
			for (int j = i + 1; j < A.size() && j <= i + t; ++j){
					if (A[j] - A[i] >= d) {
							ok = true;
					}
			}
			if (ok) ++res;
		}
		return res;
}


void test_random(){
		int n = 51;
		int interations = 100000;
		vector<int> A(n);
		for (int i = 0; i < n; ++i) A[i] = i + 1;

		RangeList<int> h;

		for (int i = 0; i < interations; ++i){
				cout<<"Test "<<i<<": ";
				random_shuffle(A.begin(), A.end());
				vector<int> backupA = A;
				h.preprocess(A);
				assert(A == backupA);
				for (int t = 2; t <= n; ++t)
					for (int d = 2; d <= n; ++d){
							long long got = h.query(t, d);
							long long exp = brute_force(A, t, d);
							if (got != exp){
									cout<<"t = "<<t<<" d = "<<d<<endl;
									for (int k = 0; k < n; ++k) cout<<A[k]<<", ";
									cout<<endl;
									cout<<"got = "<<got<<" exp = "<<exp<<endl;
							}
							assert (got == exp);
					}	
				cout<<"OK!"<<endl;
		}
		cout<<"Successful!"<<endl;
}

void test_bad_case1(){
		vector<int> A;
		A.push_back(5);
		A.push_back(2);
		A.push_back(1);
		A.push_back(4);
		A.push_back(3);
		RangeList<int> h;
		h.preprocess(A);
		int t = 2, d = 2;
		int got = h.query(t, d);
		int exp = brute_force(A, t, d);
		if (got != exp){
				cout<<"got = "<<got<<" exp = "<<exp<<endl;
		}
		assert(got == exp);
}

int main(){
		test_random();
		test_bad_case1();
}
