#!/bin/bash

function beginnings {
echo "Starting beginnings processing..."
rm -rf sandbox_beg/
mkdir sandbox_beg/
make -f Makefile-beg clean && make -f Makefile-beg
cd sandbox_beg
ln -s ../beg.exe beg.exe
ln -s ../bigtable_maker.py bigtable_maker.py
# Small output 
echo "BEG: Small with rmqBucket"
./beg.exe OUTSIZE_SMALL RMQ_BUCKET >> beg_rmqBucket.out
echo "BEG: Small with fpair"
./beg.exe OUTSIZE_SMALL FPAIR >> beg_fpair.out
echo "BEG: Small with rmqSt"
./beg.exe OUTSIZE_SMALL RMQ_ST >> beg_rmqSt.out
echo "BEG: Small with AllPairs_fpair"
./beg.exe OUTSIZE_SMALL ALLPAIRS_FPAIR >> beg_allpairs_fpair.out
# Large output
echo "BEG: Large with rmqBucket"
./beg.exe OUTSIZE_LARGE RMQ_BUCKET >> beg_rmqBucket.out
echo "BEG: Large with fpair"
./beg.exe OUTSIZE_LARGE FPAIR >> beg_fpair.out
echo "BEG: Large with rmqSt"
./beg.exe OUTSIZE_LARGE RMQ_ST >> beg_rmqSt.out
echo "BEG: Large with AllPairs_fpair"
./beg.exe OUTSIZE_LARGE ALLPAIRS_FPAIR >> beg_allpairs_fpair.out

cd ..
echo "Done."
}


function allpairs {
echo "Starting allpairs processing..."
rm -rf sandbox_all/
mkdir sandbox_all/
make -f Makefile-all clean && make -f Makefile-all
cd sandbox_all/
ln -s ../all.exe all.exe
ln -s ../bigtable_maker.py bigtable_maker.py
# Small output 
echo "ALL: Small with rmqBucket"
./all.exe OUTSIZE_SMALL RMQ_BUCKET >> all_rmqBucket.out
echo "ALL: Small with fpair"
./all.exe OUTSIZE_SMALL HFPAIR >> all_fpair.out
echo "ALL: Small with rmqSt"
./all.exe OUTSIZE_SMALL RMQ_ST >> all_rmqSt.out
# Large output
echo "ALL: Large with rmqBucket"
./all.exe OUTSIZE_LARGE RMQ_BUCKET >> all_rmqBucket.out
echo "ALL: Large with fpair"
./all.exe OUTSIZE_LARGE HFPAIR >> all_fpair.out
echo "ALL: Large with rmqSt"
./all.exe OUTSIZE_LARGE RMQ_ST >> all_rmqSt.out

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
