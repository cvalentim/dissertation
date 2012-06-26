/*
 * An implementation of the 1D ortogonal range search tree.
 * This data structure is useful for answering range
 * queries over a set of points quickly. Formally, given
 * a set of points S = {p_1, p_2, ..., p_n}, a 1D range
 * query is defined by an interval [lo, hi]. The query answer
 * must be all points p \in S such that lo <= p <= hi.
 *
 * Without preprocessing, it is clear that it is optimal 
 * to check for each p \in S if it is in [lo, hi].
 * This simple strategy gives us an <O(0), O(n)> algorithm (that
 * means, no preprocessing time and linear query answer).
 *
 * The Ortogonal Range Tree is a structure which achieves a better
 * query time by preprocessing the input sequence. The preprocessing
 * step takes O(\n log n) time and needs to keep in memory a structure
 * of size O(n), with that structure, the query can be answered in O(\log n + k)
 * time, where $k$ is the amount of solutions.
 *
 * The main ideia is to, first of all, sort the set S in increasing order, 
 * and then store those points in the leaves of a balanced search tree. Each
 * node in this tree is labeled with the largest key appearing in its left
 * subtree. Moreover, we can associate each node of this tree (implicity or 
 * explicitly) with the subset of points stored in the leaves that are
 * descendent of this node. For instance, given the sequence S = {2, 5, 7, 10},
 * the tree could look like below :
 *  
 *                   
 *        5       
 *     /     \    
 *   2        7      
 *  /  \    /  \     
 * 2    5   7  10  
 *
 * How much memory does this structure take? Well, supposing $n$ is power of two, in the 
 * last level we have $n$ nodes, in the one above it $n/2$, the next $n/4$, until the
 * first level, which has $1$ node. So, the total number of nodes is:
 *
 *  $n + n/2 + n/4 + .. + 1 < n + n/2 + n/4 + .... = n ( 1 + 1/2 + 1/4 + ..) < 2n$.
 *
 * So the total memory the range tree takes is limited by $2n$. Nice.
 * But, how do we answer the range queries from this structure? For it, we
 * need to find to special leaves:
 *
 *  1)  u - the leftmost leaf whose key is greater than or equal to lo
 *  2)  v - the rightmost leaf whose key is smaller than or equal to hi
 *
 * Clearly all leaves between (exclusive) u and v are solution for the [lo, hi] query.
 * And if p_u >= lo, we add $u$ to the answer, in the same way, if $p_v <= hi$,
 * we add $v$.
 *
 * Now, what is left is how to find the nodes u and v quickly. But, since we dealing
 * with a balanced binary search tree, doing that in O(\log n) time is a standart textbook exercise.
 */
#ifndef __RANGE_TREE__
#define __RANGE_TREE__

#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <cstdio>
#include <cassert>

#include "fastSpecialPairs.cpp"

using namespace std;

template<class T>
struct TNode
{
	int  value;
	vector<int> range;

	TNode(){}

	TNode(int _value){
		value = _value;
		range.clear();
	}

	~TNode(){
		range.clear();
	}

	friend ostream& operator<<(ostream& out, const TNode<T>& x){
		out<<x.value;
		return out;
	}		
};

template<class T>
inline bool ltByDesviation(const SpecialPair<T>& a, const SpecialPair<T>& b){
	T desvA = a.getDesviation();
	T desvB = b.getDesviation();
	if (desvA != desvB) return desvA < desvB;
	if (a.s != b.s) return a.s < b.s;
	return a.e < b.e;
}

template<class T>
class RangeTree
{
	vector<TNode<T> > tree;
	vector<vector<int> > pTable;
	vector<SpecialPair<T> > specialPairs;
	vector<bool> leaf;

public:
	RangeTree(){}

	~RangeTree(){
		tree.clear();
	}

	int getLeft(int x){
		return 2*x;
	}

	int getRight(int x){
		return 2*x + 1;
	}
	
	bool isLeaf(int x){
		return leaf[x];
	}

	int construct(int lo, int hi, int root){
		//cout<<"lo = "<<lo<<" hi = "<<hi<<" root = "<<root<<endl;
		if (lo == hi) return -10000000; 
		if (lo + 1 ==  hi) {
			tree[root].value = lo;
			pTable[lo].push_back(root);
			leaf[root] = true;
			return lo;
		}
		int m = (lo + hi - 1)/2 + 1;
		int left = construct(lo, m, getLeft(root));
		int right = construct(m, hi, getRight(root));
		tree[root].value = left; 
		for (int x = lo; x < hi; ++x)
			pTable[x].push_back(root);
		return max(left, right);
	}

	// Range Tree code begins here
	void  createRangeTree(int n){
		tree.resize(4*n + 1);
		leaf.resize(4*n + 1);
		for (int i = 0; i < 4*n + 1; ++i){
			leaf[i] = false;
			tree[i].range.clear();
		}
		construct(0, n, 1);
	}

	void remove(int root, vector<int>& hash){
		vector<int>& range = tree[root].range;
		vector<int> uniqEnds;
		for (int i = 0; i < range.size(); ++i){
			int spIndex= range[i];
			int end = specialPairs[spIndex].e;
			// only store index to special pairs
			// with unique index
			if (hash[end] != root){
				hash[end] = root;
				uniqEnds.push_back(spIndex);
			}
		}
		range.resize(uniqEnds.size());
		for (int i = 0; i < uniqEnds.size(); ++i) range[i] = uniqEnds[i];
		uniqEnds.clear();
		if (isLeaf(root)) return;
		remove(getLeft(root), hash);
		remove(getRight(root), hash);
	}

	void removeDuplicatedEnds(int n){
		vector<int> hash(n + 1, -1);
		remove(1, hash);
	}

	void preprocess(const vector<T>& seq){
		setbuf(stdout, NULL);
		specialPairs.clear();
		genSpecialPairs(seq, specialPairs);
		pTable.resize(seq.size() + 1);
		for (int i = 0; i < pTable.size(); ++i) pTable[i].clear();
		createRangeTree(seq.size());
		sort(specialPairs.rbegin(), specialPairs.rend(), ltByDesviation<T>);
		for (int i = 0; i < specialPairs.size(); ++i){
			int t = specialPairs[i].getDelta();
			for (int p = 0; p < pTable[t].size(); ++p){
				int node = pTable[t][p];
			//	cout<<"tree.size = "<<tree.size()<<" node = "<<node<<endl;
				tree[node].range.push_back(i);
			}
		}
		removeDuplicatedEnds(seq.size());

		/*cout<<"PREPROCESSING tree = "<<endl;
		for (int i = 0; i < tree.size(); ++i)
			cout<<tree[i]<<" ";
		cout<<endl;
		cout<<"seq.size = "<<seq.size()<<endl;
		cout<<"lists = "<<endl;
		for (int i = 0; i < tree.size(); ++i){
			cout<<"root = "<<i<<" :";
			for (int j = 0; j < tree[i].range.size(); ++j)
				cout<<specialPairs[tree[i].range[j]]<<" ";
			cout<<endl;
		}*/
	}

	// QUERY ROUTINES
	void add(int node, T d, vector<int>& res){
		vector<int>& range = tree[node].range;
		for (int i = 0; i < range.size(); ++i){
			int spIndex = range[i];
			if (specialPairs[spIndex].getDesviation() < d)
				break;
			res.push_back(specialPairs[spIndex].e);
		}
	}

	void addList(int root, int t, T d, vector<int>& res){
		if (isLeaf(root)){
			if (tree[root].value <= t)
				add(root, d, res);
			return;
		}
		int left = getLeft(root);
		add(left, d, res);
	}

	void findRightPath(int t, T d, vector<int>& res){
		int root = 1;
		//cout<<"finding right path="<<endl;
		while (!isLeaf(root)){
		//	cout<<"root = "<<root<<endl;
			if (tree[root].value > t)
				root = getLeft(root);
			else{
				addList(root, t, d, res);
				root = getRight(root);
			}
		}
		addList(root, t, d, res);
	}

	// [l0, l1] 
	// Returns the endings of fpairs that are inside the
	// range [0, l1]
	vector<int> queryEnds(int t, T d, vector<int>& res){
		//find_by_right_path(l1, res);
		findRightPath(t, d, res);
		// we can find more the once the same
		// f-pair, so we remove repetitions now,
		// before returning
		sort(res.begin(), res.end());
		res.erase(unique(res.begin(), res.end()), res.end());
		return res;
	}
}; 
#endif
