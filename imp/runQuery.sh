#!/bin/bash

function beginnings {
	echo "Starting beginnings processing..."
	rm -rf sandbox_beg/
	mkdir sandbox_beg/
	[ -f beg.exe ] && make -f Makefile-beg clean && make -f Makefile-beg || make -f Makefile-beg
	cd sandbox_beg
	ln -s ../beg.exe beg.exe
	ln -s ../bigtable_maker.py bigtable_maker.py
	ln -s ../pretime_maker.py pretime_maker.py
	ln -s ../plot.py plot.py
	ln -s ../plotEnds.py plotEnds.py
	# Small output 
	echo "BEG: Small with rmqBucket"
	./beg.exe OUTSIZE_SMALL RMQ_BUCKET >> beg_rmqBucket.out
	echo "BEG: Small with fpair"
	./beg.exe OUTSIZE_SMALL FPAIR >> beg_fpair.out
	echo "BEG: Small with rmqSt"
	./beg.exe OUTSIZE_SMALL RMQ_ST >> beg_rmqSt.out
	echo "BEG: Small with AllPairs_fpair"
	./beg.exe OUTSIZE_SMALL ALLPAIRS_FPAIR >> beg_allpairs_fpair.out
	echo "BEG: Small with Naive"
	./beg.exe OUTSIZE_SMALL NAIVE >> beg_naive.out
	# Large output
	echo "BEG: Large with rmqBucket"
	./beg.exe OUTSIZE_LARGE RMQ_BUCKET >> beg_rmqBucket.out
	echo "BEG: Large with fpair"
	./beg.exe OUTSIZE_LARGE FPAIR >> beg_fpair.out
	echo "BEG: Large with rmqSt"
	./beg.exe OUTSIZE_LARGE RMQ_ST >> beg_rmqSt.out
	echo "BEG: Large with AllPairs_fpair"
	./beg.exe OUTSIZE_LARGE ALLPAIRS_FPAIR >> beg_allpairs_fpair.out
	echo "BEG: Large with Naive"
	./beg.exe OUTSIZE_LARGE NAIVE >> beg_naive.out
	cat beg_rmqBucket.out beg_fpair.out beg_rmqSt.out beg_allpairs_fpair.out beg_naive.out | python2.6 bigtable_maker.py | tee begQuery.out | column -t -s , | tee begQuery-`date +"%y-%m-%d"`.out 
	python2.6 plot.py BEG
	python2.6 plotEnds.py
	cd ..
	echo "Done."
}


function allpairs {
	echo "Starting allpairs processing..."
	rm -rf sandbox_all/
	mkdir sandbox_all/
	[ -f all.exe ] && make -f Makefile-all clean && make -f Makefile-all || make -f Makefile-all
	cd sandbox_all/
	ln -s ../all.exe all.exe
	ln -s ../bigtable_maker.py bigtable_maker.py
	ln -s ../pretime_maker.py pretime_maker.py
	ln -s ../plot.py plot.py
	# Small output 
	echo "ALL: Small with rmqBucket"
	./all.exe OUTSIZE_SMALL RMQ_BUCKET >> all_rmqBucket.out
	echo "ALL: Small with fpair"
	./all.exe OUTSIZE_SMALL HFPAIR >> all_fpair.out
	echo "ALL: Small with rmqSt"
	./all.exe OUTSIZE_SMALL RMQ_ST >> all_rmqSt.out
	echo "ALL: Small with naive"
	./all.exe OUTSIZE_SMALL NAIVE >> all_naive.out

	# Large output
	echo "ALL: Large with rmqBucket"
	./all.exe OUTSIZE_LARGE RMQ_BUCKET >> all_rmqBucket.out
	echo "ALL: Large with fpair"
	./all.exe OUTSIZE_LARGE HFPAIR >> all_fpair.out
	echo "ALL: Large with rmqSt"
	./all.exe OUTSIZE_LARGE RMQ_ST >> all_rmqSt.out
	echo "ALL: Large with naive"
	./all.exe OUTSIZE_LARGE NAIVE >> all_naive.out

	cat all_rmqBucket.out all_fpair.out all_rmqSt.out all_naive.out | python2.6 bigtable_maker.py | tee allQuery.out | column -t -s , | tee allQuery-`date +"%y-%m-%d"`.out
	python2.6 plot.py ALL
	cd ..
	echo "Done."
}

if [ "$1" == "BEG" ]; then
	beginnings
elif [ "$1" == "ALL" ]; then
	allpairs
else
	echo "Invalid parameter."
fi
