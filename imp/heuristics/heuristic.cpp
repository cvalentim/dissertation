#ifndef __HEURISTIC__
#define __HEURISTIC__

#include <iostream>
#include <vector>
#include <string>
using namespace std;

template<class T>
class Heuristic
{
public:
	//WARNING: fix me!
	virtual int get_presize() = 0;
	virtual string get_name() = 0;
	virtual void preprocess(vector<T>& A) = 0;
	virtual long long query(int t, T d) = 0;
};
#endif
