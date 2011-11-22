#ifndef __DATASET_HANDLER__
#define __DATASET_HANDLER__

#include <vector>
#include <iostream>
#include <fstream>
#include <cstdio>

using namespace std;

template<class T>
class DataHandler{
protected:
	vector<vector<T> > data_set;
	int ndata_sets;
	virtual string get_data_path(string path, int ns) = 0;
public:
	~DataHandler(){
			data_set.clear();
	}

	virtual void load_dataset(string path) {
			if (path.empty()) {
					cerr<<"load_dataset: Cannot load empty path"<<endl;
					return;
			}
			data_set.clear();
			if (path[path.size() - 1] != '/') path += "/";
			
			for (int ns = 1; ns <= ndata_sets; ++ns){
				string abs_path = get_data_path(path, ns);
				ifstream fin; fin.open(abs_path.c_str());
				if (!fin){
						cerr<<"load_dataset: Cannot open "<<abs_path<<endl;
						return;
				}
				T value;
				vector<T> data;
				while (fin >> value) data.push_back(value);
				fin.close();
				data_set.push_back(data);
			}	
	}

	virtual vector<T> get_next() {
			static int next = 0;
			if (next == ndata_sets) return vector<T>();
			return data_set[next++];
	}
};

template<class T>
class RandomDataHandler: public DataHandler<T>{
protected:
	virtual string get_data_path(string path, int ns){
				char abs_path[100];
				sprintf(abs_path, "%srandom_serie%d.in", path.c_str(), ns);
				return string(abs_path);
	}

public:
	RandomDataHandler(){
			DataHandler<T>::ndata_sets = 4;
	}

};

template<class T>
class RealDataHandler: public DataHandler<T>{
	virtual string get_data_path(string path, int ns){
				char abs_path[100];
				sprintf(abs_path, "%sserie%d.in", path.c_str(), ns);
				return string(abs_path);
	}
public:
	RealDataHandler(){
			DataHandler<T>::ndata_sets = 6;
	}
};


#endif
