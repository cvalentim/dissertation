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
#ifndef __RANGE_TREE_FPAIR__
#define __RANGE_TREE_FPAIR__

#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <cstdio>

#include "fpair.cpp"

using namespace std;

template<class T>
struct TNode{
		int  value;
		vector<int> range;
		bool is_leaf;

		TNode(){
			is_leaf = false;
		}

		TNode(int _value){
			value = _value;
			is_leaf = false;
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
bool lt_by_time(const FPair<T>& f1, const FPair<T>& f2){
		if (f1.get_delta_t() != f2.get_delta_t())
				return f1.get_delta_t() < f2.get_delta_t();
		if (f1.s != f2.s)
				return f1.s < f2.s;
		return f1.e < f2.e;
}

template<class T>
class RangeTreeFPair;

template<class T>
bool lt_lexico(const FPair<T>& f1, const FPair<T>& f2){
		if (f1.s != f2.s) return f1.s < f2.s;
		return f1.e < f2.e;
}


template<class T>
struct lt_by_end:std::binary_function<int , int, bool>
{
   	lt_by_end(RangeTreeFPair<T> *p):rangeTree(p) {}

    bool operator() (const int& index1, const int& index2) {
				if (rangeTree->seq[index1].e != rangeTree->seq[index2].e)
						return rangeTree->seq[index1].e < rangeTree->seq[index2].e;
				return rangeTree->seq[index1].s < rangeTree->seq[index2].s;
    }
    RangeTreeFPair<T> *rangeTree;
};

template<class T>
struct lt_by_value:std::binary_function<int , int, bool>
{
   	lt_by_value(RangeTreeFPair<T> *p):rangeTree(p) {}

    bool operator() (const int& index1, const int& index2) {
				if (rangeTree->seq[index1].get_delta_v() != rangeTree->seq[index2].get_delta_v())
						return rangeTree->seq[index1].get_delta_v() < rangeTree->seq[index2].get_delta_v();
				if (rangeTree->seq[index1].s != rangeTree->seq[index2].s)
						rangeTree->seq[index1].s <  rangeTree->seq[index2].s;
				return rangeTree->seq[index1].e < rangeTree->seq[index2].e;
    }
    RangeTreeFPair<T> *rangeTree;
};


// Is this the correct way of doing binary predicate?
template<class T>
struct equal_end:std::binary_function<int , int, bool>
{
   	equal_end(RangeTreeFPair<T> *p):rangeTree(p) {}

    bool operator()(const int& index1, const int& index2) {
				return rangeTree->seq[index1].e == rangeTree->seq[index2].e;
    }
    RangeTreeFPair<T> *rangeTree;
};

template<class T>
class RangeTreeFPair{

		vector<TNode<T> > tree;
		
		vector<FPair<T> > seq;

		int l1;

		T hi;

		int get_left(int x){
				return 2 * x;
		}
	
		int get_right(int x){
				return 2 * x + 1;
		}	
	
		bool is_leaf(int node){
				return tree[node].is_leaf;
		}

		bool isValid(const FPair<T>& f){
					return 0 <= f.get_delta_t() && f.get_delta_t() <= l1 && f.get_delta_v() >= hi;
		}

		void PrintPath(const vector<int>& path){
					for (int i = 0; i < path.size(); ++i)
							cout<<tree[path[i]]<<" -> ";
					cout<<endl;
		}

		friend class equal_end<T>;
		friend class lt_by_value<T>;
		friend class lt_by_end<T>;
		int total_ends, total_ends_unique;
public:

		RangeTreeFPair(){
			total_ends = total_ends_unique = 0;
		}

		~RangeTreeFPair(){
				seq.clear();
				tree.clear();
		}
	
		// semi-open interval [s, e)
		int construct(int s, int e, int node){
				// base case: an leaf.
				// In this case the subtree has only one node
				if (s >= e){
						assert (0 == 1);
				}
				if (s + 1 == e){
						tree[node] = TNode<T>(seq[s].get_delta_t());
						tree[node].range.push_back(s);
						tree[node].is_leaf = true;
						return seq[s].get_delta_t();
				}
				// Otherwise, it is internal node
				// So it value will be smallest in the left subtree,
				// which we get by recursive calling
				int m = (s + e)/2;

				// associates with this node a list of fpairs 
				// in the interval [s, e) sorted decreasing by delta_v 
				//for (int k = s; k < e; ++k)
			  //	tree[node].range.push_back(seq[k]);
			  vector<int>& range = tree[node].range;
				for (int k = s; k < e; ++k)
						range.push_back(k);

				total_ends += (e - s);

				// Remove equal ends
				sort(range.begin(),range.end(), lt_by_end<T>(this));
				typename vector<int>::iterator it = unique(range.begin(),range.end(), equal_end<T>(this));
				range.erase(it, range.end());

				total_ends_unique += range.size();

				// sort by increasing delta_v 
				sort(range.rbegin(), range.rend(), lt_by_value<T>(this));
				
				int greatest_left = construct(s, m, 2*node);
				int greatest_right = construct(m, e, 2*node + 1);

				// the node value is the smallest in the left subtree,
				// node value is used to discover the canonics interval
				// which represents a query [lo, l1]
				tree[node].value = greatest_left;
			
				// we need to keep the recursive invariant true, 
				// so we return the largest Fpair(in terms of delta t) in this subtree
				return max(greatest_left, greatest_right);
		}

		void add_left(int curr, vector<int>& res){
				int node = get_left(curr);
				const vector<int >& range = tree[node].range;
				for (int i = 0; i < range.size(); ++i){
							if (seq[range[i]].get_delta_v() < hi)
													break;
							res.push_back(seq[range[i]].e);
				}
		}
	
	
		void find_by_right_path(int x, vector<int >& res){
				// starts at the root
				int curr = 1;
				while (!is_leaf(curr)){
						if (tree[curr].value > x)
							curr = get_left(curr);	
						else{
							// goes right, so add solutions
							// in the left subtree of the
							// current node
							add_left(curr, res);
							curr = get_right(curr);
						}
				}
				// curr is a leaf
				assert (tree[curr].range.size() == 1);
				if (curr >= tree.size()){
					printf("WARNING: curr = %d len(tree) = %d\n", curr, (int)tree.size());
				}
				if (isValid(seq[tree[curr].range[0]]))
							res.push_back(seq[tree[curr].range[0]].e);
		}


		// Range Tree code begins here
		void range_preprocess(const vector<FPair<T> >& A){
				if (A.empty())
						return;
				seq = A;
				tree.clear();
				tree.resize(4*(int)seq.size() + 1);
				sort(seq.begin(), seq.end(), lt_by_time<T>);
				int n = seq.size();
				construct(0, n, 1);
				//printf("ends = %d ends_unique = %d delta = %d\n", total_ends, total_ends_unique, total_ends - total_ends_unique);
		}

	
		// [l0, l1] 
		// Returns the endings of fpairs that are inside the
		// range [0, l1]
		vector<int> range_query(int _l1, T _hi, vector<int>& res){
					l1 = _l1, hi = _hi;
					find_by_right_path(l1, res);
					// we can find more the once the same
					// f-pair, so we remove repetitions now,
					// before returning
					sort(res.begin(), res.end());
					res.erase(unique(res.begin(), res.end()), res.end());				
					return res;
		}
}; 
#endif
