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

#include "fpair.cpp"

using namespace std;

template<class T>
struct TNode{
		int  value;
		vector<FPair<T> > range;
		bool is_leaf;

		TNode(){
			is_leaf = false;
		}

		TNode(int _value){
			value = _value;
			is_leaf = false;
		}

		virtual ~TNode(){
				range.clear();
		}

		friend ostream& operator<<(ostream& out, const TNode<T>& x){
				out<<x.value;
				return out;
		}		
};

template<class T>
struct LNode: public TNode<T>{
		int index;
		LNode(int _value, int _index): TNode<T>(_value), index(_index){	}
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
bool lt_by_value(const FPair<T>& f1, const FPair<T>& f2){
		if (f1.get_delta_v() != f2.get_delta_v())
				return f1.get_delta_v() < f2.get_delta_v();
		if (f1.s != f2.s)
				return f1.s < f2.s;
		return f1.e < f2.e;
}

template<class T>
bool lt_lexicographic(const FPair<T>& f1, const FPair<T>& f2){
		if (f1.s != f2.s) return f1.s < f2.s;
		return f1.e < f2.e;
}

template<class T>
bool lt_by_end(const FPair<T>& f1, const FPair<T>& f2){
	if (f1.e != f2.e) return f1.e < f2.e;
	return f1.s < f2.s;
}

template<class T>
bool equal_end(const FPair<T>& f1, const FPair<T>& f2){
		return f1.e == f2.e;		
}

template<class T>
class RangeTreeFPair{
		vector<FPair<T> > seq;
		vector<TNode<T> > tree;
		int l0, l1;
		T hi;

public:
		RangeTreeFPair(){}
		~RangeTreeFPair(){
				seq.clear();
				tree.clear();
		}
	
		// semi-open interval [s, e)
		int construct(int s, int e, int node){
				// base case: an leaf.
				// In this case the subtree has only one node
				if (s + 1 == e){
						tree[node] = LNode<T>(seq[s].get_delta_t(), s);
						tree[node].range.push_back(seq[s]);
						tree[node].is_leaf = true;
						return seq[s].get_delta_t();
				}
				// Otherwise, it is internal node
				// So it value will be smallest in the left subtree,
				// which we get by recursive calling
				int m = (s + e)/2;

				// associates with this node a list of fpairs 
				// in the interval [s, e) sorted decreasing by delta_v 
				for (int k = s; k < e; ++k)
						tree[node].range.push_back(seq[k]);

				// TODO: Remove equal ends
				sort(tree[node].range.begin(), tree[node].range.end(), lt_by_end<T>);
				typename vector<FPair<T> >::iterator it = unique(tree[node].range.begin(), tree[node].range.end(), equal_end<T>);
				tree[node].range.erase(it, tree[node].range.end());

				// sort by increasing delta_v 
				sort(tree[node].range.begin(), tree[node].range.end(), lt_by_value<T>);
				reverse(tree[node].range.begin(), tree[node].range.end());
				
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

		bool is_leaf(int node){
				return tree[node].is_leaf;
		}

		// Find the left path from the root
		// to the leftmost node greater or equal to x.
		vector<int> find_left_path(int x, int node){
					int curr = node;
					vector<int> path;
					path.push_back(curr);
					while (!is_leaf(curr)){
							if (tree[curr].value < x)
									curr = 2*curr + 1;
							else
									curr = 2*curr;
							path.push_back(curr);
					}
					return path;
		}

		// Find the rightmost path from the root
		// to the rightmost node smallest or equal to x,
		vector<int> find_right_path(int x, int node){
				int curr = node;
				vector<int> path;
				path.push_back(node);
				while (!is_leaf(curr)){
						if (tree[curr].value > x)
							curr = 2*curr;	
						else
							curr = 2*curr + 1;
						path.push_back(curr);
				}
				return path;
		}

		void find_left_solutions(const vector<int>& lpath, int nleft, vector<FPair<T> >& res){	
				// find solutions in the left path
				while (nleft < lpath.size()){
							int curr = lpath[nleft];
							// we are in a leaf
							if (is_leaf(curr)) {
									assert (tree[curr].range.size() == 1);
									if (isValid(tree[curr].range[0]))
											res.push_back(tree[curr].range[0]);
									return;
							}
							int next = lpath[nleft + 1];
							// we go right, don't need
							// to add the solutions of the 
							// current node
							if (2*curr + 1 == next){
										++nleft;
										continue;	
							}

							// now we try to find all f-pair in the
							// node which respect the delta_v restriction
							const vector<FPair<T> >& range = tree[2*curr + 1].range;
							for (int i = 0; i < range.size(); ++i){
									if (range[i].get_delta_v() < hi)
												break;
									res.push_back(range[i]);
							}
							++nleft;
				}
		}

		
		void find_right_solutions(const vector<int>& rpath, int nright, vector<FPair<T> >& res){
				// find solutions in the right path
					while (nright < rpath.size()){
								int curr = rpath[nright];
								// path end, we are on a leaf
								if (is_leaf(curr)) {
										assert (tree[curr].range.size() == 1);
										if (isValid(tree[curr].range[0]))
												res.push_back(tree[curr].range[0]);
										return;
								}
								int next = rpath[nright + 1];

								// we go left, so don't
								// add solutions from the current node
								if (2*curr == next){
											++nright;
											continue;
								}

								// Otherwise, we find all solutions in the
								// left subtree of the current node
								const vector<FPair<T> >& range = tree[2*curr].range;
								for (int i = 0; i < range.size(); ++i){
										if (range[i].get_delta_v() < hi)
													break;
										res.push_back(range[i]);
								}
								++nright;
					}
		}


		// Range Tree code begins here
		void range_preprocess(const vector<FPair<T> >& A){
				if (A.empty()){
						return;
				}
				seq = A;
				tree.clear();
				tree.resize(seq.size() * 4);
				sort(seq.begin(), seq.end(), lt_by_time<T>);
				int n = seq.size();
				construct(0, n, 1);
		}

		bool isValid(const FPair<T>& f){
					return l0 <= f.get_delta_t() && f.get_delta_t() <= l1 && f.get_delta_v() >= hi;
		}

		void PrintPath(const vector<int>& path){
					for (int i = 0; i < path.size(); ++i)
							cout<<tree[path[i]]<<" -> ";
					cout<<endl;
		}
		
		// [l0, l1] 
		vector<FPair<T> > range_query(int _l0, int _l1, T _hi){
					// Invariant: There must be at least in the range tree
					l0 = _l0, l1 = _l1, hi = _hi;
					
					vector<int> lpath, rpath;

					lpath = find_left_path(l0, 1);
					rpath = find_right_path(l1, 1);

					// Invariant: there must be at least one
					// node (the root) in both paths
					assert (lpath.size() > 0 && rpath.size() > 0);

					int nleft = 0, nright = 0;
					while (nleft < lpath.size() && nright < rpath.size()){
							if (lpath[nleft] == rpath[nright])
									++nleft, ++nright;
							else 
									break;
					}
			
					vector<FPair<T> > res;
					if (nleft == lpath.size()){
							int leaf = lpath[nleft - 1];
							FPair<T> f = tree[leaf].range[0];
							if (isValid(f))
									res.push_back(f);
							return res;
					}

					find_left_solutions(lpath, nleft, res);
					find_right_solutions(rpath, nright, res);
	
					// we can find more the once the same
					// f-pair, so we remove repetitions now,
					// before returning
					sort(res.begin(), res.end(), lt_lexicographic<T>);
					res.erase(unique(res.begin(), res.end()), res.end());				

					return res;
		}
}; 


#endif
