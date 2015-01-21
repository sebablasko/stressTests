#!/bin/bash
cd DEV_NULL
python ../post_processing_perf.py
mv perfTests.csv ../RESULTS/perfSummary_devnull.csv

cd ../DEV_URANDOM
python ../post_processing_perf.py
mv perfTests.csv ../RESULTS/perfSummary_devurandom.csv

cd ../FIFO
python ../post_processing_perf.py
mv perfTests.csv ../RESULTS/perfSummary_fifo.csv

cd ../TCP
python ../post_processing_perf.py
mv perfTests.csv ../RESULTS/perfSummary_tcp.csv

cd ../UDP
python ../post_processing_perf.py
mv perfTests.csv ../RESULTS/perfSummary_udp.csv

cd ../UNIX
python ../post_processing_perf.py
mv perfTests.csv ../RESULTS/perfSummary_unix.csv