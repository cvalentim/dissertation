#ifndef __PRE_ALL__
#define __PRE_ALL__

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <algorithm>
#include <cstring>
#include <string>

#include "../heuristics/allpairs/h_fpairs.cpp"
#include "../heuristics/allpairs/pure_rmq.cpp"
#include "../heuristics/heuristic.cpp"

#include "../data_set_handler.cpp"

// just for the fpair
#include "../heuristics/beginnings/range_list/fpair.cpp"

#include "../timer/clock.cpp"

using namespace std;

#define RMQ_BUCKET 0
#define RMQ_ST 1
#define HFPAIRS 2


int main(int argc, char* argv[])
{
	int type_h, outsize;

	DataHandler<double> *data_set = new RealDataHandler<double>();
	data_set->load_dataset("/home/cvalentim/Mestrado/research/data_structures/instances/real_data/data/");

	vector<Heuristic<double>* > hs;
    hs.push_back(new PureRmq<double>(new RMQBucket<double>(RMQTypes_t::MAX())));
    hs.push_back(new PureRmq<double>(new RMQSt<double>(RMQTypes_t::MAX())));

    hs.push_back(new HFPairs<double>());
    int nr;
    if (argc > 1)
        nr = atoi(argv[1]);
    else 
        nr = 100;
    Clock clock = Clock();
    vector<string> header;
    vector<vector<double> > table;
    vector<int> sizes;
    table.resize(13);
    sizes.resize(13);
    vector<vector<double> > corpus;
    while (1){
        vector<double> seq = data_set->get_next();
        if (seq.empty()) break;
        corpus.push_back(seq);
    }
    for (int i = 0; i < hs.size(); ++i){
        Heuristic<double> *h = hs[i];
        header.push_back(h->get_name());
        cout<<"heuristic = "<<i<<endl;
        int serie = 1;
		for (int c = 0; c < corpus.size(); ++c){
				vector<double>& data = corpus[c];
				if (data.empty()) break;
                double av_time = 0;
                for (int k = 0; k < nr; ++k){
				    clock.start();
                    h->preprocess(data);
                    clock.end();
                    av_time += clock.elapsed();
				}
                table[serie].push_back(av_time/nr);
                sizes[serie] = (int)data.size();
				++serie;
		}
    }
    cout<<"SerieID, SerieSize, fpairs, "<<header[0]<<", "<<header[1]<<", "<<header[2]<<endl;
    for (int s = 1; s <= 12; ++s){
        int amf = amount_fpairs(corpus[s - 1]);
        cout<<s<<", "
            <<sizes[s]<<", "
            <<amf<<", "
            <<table[s][0]<<", "
            <<table[s][1]<<", "
            <<table[s][2]<<endl;
                
    }

	return 0;
}
#endif
