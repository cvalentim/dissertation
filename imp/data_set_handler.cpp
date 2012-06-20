#ifndef __DATASET_HANDLER__
#define __DATASET_HANDLER__

#include <vector>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <algorithm>

using namespace std;

// Abstract class to handle a dataset type
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
			// make sure the path ends with "/" for directory
			if (path[path.size() - 1] != '/') path += "/";

			for (int ns = 1; ns <= ndata_sets; ++ns){
				string abs_path = get_data_path(path, ns);
				ifstream fin; fin.open(abs_path.c_str());
				if (!fin){
					cerr<<"load_dataset: Cannot open "<<abs_path<<endl;
					continue;
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
class RealDataHandler: public DataHandler<T>{
	virtual string get_data_path(string path, int ns){
		char abs_path[200];
		sprintf(abs_path, "%sserie%d.in", path.c_str(), ns);
		return string(abs_path);
	}
	public:
	RealDataHandler(){
		// there are twelve different input files in the
		// real data set. The first six (1...6) are stock data from
		// the stock market, the next six (7...12) are the first
		// six series reversed, in order to simulate the negative query.
		DataHandler<T>::ndata_sets = 12;
	}
};


#endif
