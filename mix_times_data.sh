#!/bin/bash
cd RESULTS

echo "" > all_times_data.csv

for filename in *_times.csv; do
	echo $filename >> all_times_data.csv
	cat $filename >> all_times_data.csv
	echo "" >> all_times_data.csv
done