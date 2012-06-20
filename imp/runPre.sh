#!/bin/bash

function beginnings {
	echo "Starting beginnings processing..."
	rm -rf sandbox_beg_pre/
	mkdir sandbox_beg_pre/
	[ -f beg.exe ] && make -f Makefile-beg clean && make -f Makefile-beg || make -f Makefile-beg
	cd sandbox_beg_pre/
	ln -s ../beg.exe beg.exe
	ln -s ../pretime_maker.py pretime_maker.py
	ln -s ../plotPre.py plotPre.py
	./beg.exe PRE RMQ_BUCKET >> beg_rmqBucket.out
	./beg.exe PRE FPAIR >> beg_fpair.out
	./beg.exe PRE RMQ_ST >> beg_rmqSt.out
	./beg.exe PRE ALLPAIRS_FPAIR >> beg_allpairs_fpair.out
	cat beg_rmqBucket.out beg_fpair.out beg_rmqSt.out beg_allpairs_fpair.out | python pretime_maker.py | tee begPretime.out | column -t -s , | tee begPretime-`date +"%y-%m-%d"`.out 
	python2.6 plotPre.py BEG
	cd ..
	echo "Done."
}


function allpairs {
	echo "Starting allpairs processing..."
	rm -rf sandbox_all_pre/
	mkdir sandbox_all_pre/
	[ -f all.exe ] && make -f Makefile-all clean && make -f Makefile-all || make -f Makefile-all
	cd sandbox_all_pre/
	ln -s ../all.exe all.exe
	ln -s ../pretime_maker.py pretime_maker.py
	ln -s ../plotPre.py plotPre.py
	./all.exe PRE RMQ_BUCKET >> all_rmqBucket.out
	./all.exe PRE HFPAIR >> all_fpair.out
	./all.exe PRE RMQ_ST >> all_rmqSt.out
	cat all_rmqBucket.out all_fpair.out all_rmqSt.out | python pretime_maker.py | tee allPretime.out | column -t -s , | tee allPretime-`date +"%y-%m-%d"`.out
	python2.6 plotPre.py ALL
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
