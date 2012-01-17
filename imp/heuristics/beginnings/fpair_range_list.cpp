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

struct TNode{
		static int  ID;
		TNode *left, *right;

		int  value;
		int id;

		vector<FPair> range;

		TNode(){
			id = ID++;
			left = right = 0;
		}

		TNode(int _value){
			id = ID++;
			left = right = 0;
			value = _value;
		}

		virtual ~TNode(){
				range.clear();
				if (left != NULL)
					delete left;
				if (right!= NULL)
					delete right;
		}

		void _print(TNode *node, string indent){
				if (node == NULL) return;
				cout<<indent<<*node<<endl;
				_print(node->left, indent + " ");
				_print(node->right, indent + " ");
		}

		void print(){
			_print(this, "");	

		}

		bool operator==(const TNode& rhs) const{
				return id == rhs.id && value == rhs.value && left == rhs.left && right == rhs.right;
		}

		friend ostream& operator<<(ostream& out, const TNode& x){
				out<<"("<<x.id<<", "<<x.value<<", "<<x.left<<", "<<x.right<<")";
				return out;
		}		

};


struct LNode: public TNode{
		int index;
		LNode(int _value, int _index): TNode(_value), index(_index){	}
};


bool lt_by_time(const FPair& f1, const FPair& f2){
		if (f1.get_delta_t() != f2.get_delta_t())
				return f1.get_delta_t() < f2.get_delta_t();
		if (f1.s != f2.s)
				return f1.s < f2.s;
		return f1.e < f2.e;
}

bool lt_by_value(const FPair& f1, const FPair& f2){
		if (f1.get_delta_v() != f2.get_delta_v())
				return f1.get_delta_v() < f2.get_delta_v();
		if (f1.s != f2.s)
				return f1.s < f2.s;
		return f1.e < f2.e;
}

bool lt_lexicographic(const FPair& f1, const FPair& f2){
		if (f1.s != f2.s) return f1.s < f2.s;
		return f1.e < f2.e;
}

class RangeTreeFPair{
		TNode *root;
		vector<FPair> seq;
		int l0, l1;
		double hi;

public:
		RangeTreeFPair(){
				root = NULL;
		}

		~RangeTreeFPair(){
				delete root;
		}
	
		// semi-open interval [s, e)
		int construct(int s, int e, TNode** node){
				// base case: an leaf.
				// In this case the subtree has only one node
				if (s + 1 == e){
						*node = new LNode(seq[s].get_delta_t(), s);
						(*node)->range.push_back(seq[s]);
						return seq[s].get_delta_t();
				}
				// Otherwise, it is internal node
				// So it value will be smallest in the left subtree,
				// which we get by recursive calling
				int m = (s + e)/2;

				// create space in memory for a new node since
				// the pointer we received doesn't point to a valid node
				*node = new TNode();

				// associates with this node a list of fpairs 
				// in the interval [s, e) sorted decreasing by delta_v 
				for (int k = s; k < e; ++k)
						(*node)->range.push_back(seq[k]);

				// TODO: Remove equal ends
				sort((*node)->range.begin(), (*node)->range.end(), lt_by_value);
				reverse((*node)->range.begin(), (*node)->range.end());
				
				int greatest_left = construct(s, m, &((*node)->left));
				int greatest_right = construct(m, e, &((*node)->right));

				// the node value is the smallest in the left subtree,
				// node value is used to discover the canonics interval
				// which represents a query [lo, l1]
				(*node)->value = greatest_left;
			
				// we need to keep the recursive invariant true, 
				// so we return the largest Fpair(in terms of delta t) in this subtree
				return max(greatest_left, greatest_right);
		}

		bool is_leaf(TNode *node){
				return node->left == 0 && node->right == 0;
		}

		// Find the left path from the root
		// to the leftmost node greater or equal to x.
		vector<TNode *> find_left_path(int x, TNode *node){
					assert (node != NULL);
					TNode *curr = node;
					vector<TNode *> path;
					path.push_back(curr);
					while (!is_leaf(curr)){
							if (curr->value < x)
									curr = curr->right;
							else
									curr = curr->left;
							path.push_back(curr);
					}
					return path;
		}

		// Find the rightmost path from the root
		// to the rightmost node smallest or equal to x,
		vector<TNode *> find_right_path(int x, TNode *node){
				TNode *curr = node;
				vector<TNode *> path;
				path.push_back(node);
				while (!is_leaf(curr)){
						if (curr->value > x)
							curr = curr->left;	
						else
							curr = curr->right;
						path.push_back(curr);
				}
				return path;
		}

		void find_left_solutions(const vector<TNode *>& lpath, int nleft, vector<FPair>& res){	
				// find solutions in the left path
				while (nleft < lpath.size()){
							TNode *curr = lpath[nleft];
							// we are in a leaf
							if (is_leaf(curr)) {
									assert (curr->range.size() == 1);
									if (isValid(curr->range[0]))
											res.push_back(curr->range[0]);
									return;
							}
							TNode *next = lpath[nleft + 1];
							// we go right, don't need
							// to add the solutions of the 
							// current node
							if (curr->right == next){
										++nleft;
										continue;	
							}

							// now we try to find all f-pair in the
							// node which respect the delta_v restriction
							const vector<FPair>& range = curr->right->range;
							for (int i = 0; i < range.size(); ++i){
									if (range[i].get_delta_v() < hi)
												break;
									res.push_back(range[i]);
							}
							++nleft;
				}
		}

		
		void find_right_solutions(const vector<TNode *>& rpath, int nright, vector<FPair>& res){
				// find solutions in the right path
					while (nright < rpath.size()){
								TNode *curr = rpath[nright];
								// path end, we are on a leaf
								if (is_leaf(curr)) {
										assert (curr->range.size() == 1);
										if (isValid(curr->range[0]))
												res.push_back(curr->range[0]);
										return;
								}
								TNode *next = rpath[nright + 1];

								// we go left, so don't
								// add solutions from the current node
								if (curr->left == next){
											++nright;
											continue;
								}

								// Otherwise, we find all solutions in the
								// left subtree of the current node
								const vector<FPair>& range = curr->left->range;
								for (int i = 0; i < range.size(); ++i){
										if (range[i].get_delta_v() < hi)
													break;
										//printf("Adding f-pair (%d, %lf)\n", range[i].get_delta_t(),
										//																	 range[i].get_delta_v());
										res.push_back(range[i]);
								}
								++nright;
					}
		}


		// Range Tree code begins here
		void range_preprocess(const vector<FPair>& A){
				if (A.empty()){
						//cout<<"WARNING: Empty input sequence for range_preprocess"<<endl;
						return;
				}
				// WARNING: MEMORY LEAK!!! NEED TO DESTROY any previous
				// range tree
				if (root != NULL){
						delete root;
				}

				TNode::ID = 0;
				seq = A;
				sort(seq.begin(), seq.end(), lt_by_time);
				//cout<<"Sequence: ";
				//for (int i = 0; i < seq.size(); ++i)
				//		cout<<"("<<seq[i].get_delta_t()<<", "<<seq[i].get_delta_v()<<"); ";
				//cout<<endl;
				int n = seq.size();
				//if (root != NULL) delete root;
				
				construct(0, n, &root);
		}

		bool isValid(const FPair& f){
					return l0 <= f.get_delta_t() && f.get_delta_t() <= l1 && f.get_delta_v() >= hi;
		}

		void PrintPath(const vector<TNode *>& path){
					for (int i = 0; i < path.size(); ++i)
							cout<<*(path[i])<<" -> ";
					cout<<endl;
		}

		
		// [l0, l1] 
		vector<FPair> range_query(int _l0, int _l1, double _hi){
					// Invariant: There must be at least in the range tree
					if  (root == 0){
							//cout<<"WARNING: Doing range query without preprocess, returning."<<endl;
							return vector<FPair>();
					}
 
					l0 = _l0, l1 = _l1, hi = _hi;
					
					vector<TNode *> lpath, rpath;

					lpath = find_left_path(l0, root);
					rpath = find_right_path(l1, root);

					// Invariant: there must be at least one
					// node (the root) in both paths
					assert (lpath.size() > 0 && rpath.size() > 0);

					int nleft = 0, nright = 0;
					while (nleft < lpath.size() && nright < rpath.size()){
							if (*(lpath[nleft]) == *(rpath[nright]))
									++nleft, ++nright;
							else 
									break;
					}
			
					vector<FPair> res;
					//printf("Silly case?\n");
					//printf("l0 = %d l1 = %d hi = %lf\n", l0, l1, hi);
					//printf("lpath.size() = %d nleft = %d\n", lpath.size(), nleft);	
				  //printf("rpath.size() = %d nright = %d\n", rpath.size(), nright);	
					// silly case where both paths are the same,
					//root->print();
					//cout<<"Left Path: ";
					//PrintPath(lpath);
					//cout<<"Right Path: ";	
					//PrintPath(rpath);
					// that way, we have only one fpair to check
					if (nleft == lpath.size()){
						LNode *leaf = (LNode *)lpath[nleft - 1];
							//FPair fleaf = seq[leaf->index];
							//printf("index = %d\n", leaf->index);
							FPair f = leaf->range[0];
							//cout<<"f = "<<f<<endl;
							if (isValid(f))
									res.push_back(f);
							return res;
					}
				//	printf("General case\n");
					// if we're here, both diverged at some point,
					// then we have distinct left and right path
					// to work with
					find_left_solutions(lpath, nleft, res);
					find_right_solutions(rpath, nright, res);
	
					// we can find more the once the same
					// f-pair, so we remove repetitions now,
					// before returning
					sort(res.begin(), res.end(), lt_lexicographic);
					res.erase(unique(res.begin(), res.end()), res.end());				

					return res;
		}
}; 

int TNode::ID = 0;

#endif
