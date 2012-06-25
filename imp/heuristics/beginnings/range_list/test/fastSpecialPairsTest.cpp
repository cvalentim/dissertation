#include <stdio.h>
#include <vector>
#include <algorithm>

#include "../fastSpecialPairs.cpp"

using namespace std;

int main(){
	vector<int> A(4);
	A[0] = 1, A[1] = 14, A[2] = 10, A[3] = 15;
	//reverse(A.begin(), A.end());
	printf("total = %d\n", amSpecialPairs(A)); 

	return 0;
}
