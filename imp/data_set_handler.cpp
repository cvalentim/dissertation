#ifndef __DATASET_HANDLER__
#define __DATASET_HANDLER__

#include <vector>
#include <iostream>
#include <fstream>
#include <cstdio>

using namespace std;

class DataHandler{
public:
	virtual void load_dataset(string path) = 0;
	virtual vector<double> get_next() = 0;
};

class RandomDataHandler: public DataHandler{
	vector<vector<double> > data_set;
public:
	void load_dataset(string path){
			if (path.empty()) {
					cerr<<"load_dataset: Cannot load empty path"<<endl;
					return;
			}
			data_set.clear();
			if (path[path.size() - 1] != '/') path += "/";
			
			for (int ns = 1; ns <= 5; ++ns){
				char abs_path[100];
				sprintf(abs_path, "%srandom_serie%d.in", path.c_str(), ns);
				vector<double> data;
				ifstream fin; fin.open(abs_path);
				if (!fin){
						cerr<<"load_dataset: Cannot open "<<abs_path<<endl;
						return;
				}
				double value;
				vector<double> A;
				while (fin >> value) A.push_back(value);
				fin.close();
				data_set.push_back(A);
			}
	}

	vector<double> get_next(){
			static int next = 0;
			if (next == 6) return vector<double>();
			return data_set[next++];
	}
};


class RealDataHandler: public DataHandler{
	vector<vector<double> > data_set;
public:
	void load_dataset(string path){
			if (path.empty()) {
					cerr<<"load_dataset: Cannot load empty path"<<endl;
					return;
			}
			data_set.clear();
			if (path[path.size() - 1] != '/') path += "/";
			
			for (int ns = 1; ns <= 6; ++ns){
				char abs_path[100];
				sprintf(abs_path, "%sserie%d.in", path.c_str(), ns);
				vector<double> data;
				ifstream fin; fin.open(abs_path);
				if (!fin){
						cerr<<"load_dataset: Cannot open "<<abs_path<<endl;
						return;
				}
				double value;
				vector<double> A;
				while (fin >> value) A.push_back(value);
				fin.close();
				data_set.push_back(A);
			}
	}

	vector<double> get_next(){
			static int next = 0;
			if (next == 6) return vector<double>();
			return data_set[next++];
	}
};

#endif
