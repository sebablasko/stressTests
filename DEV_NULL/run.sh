#!/bin/bash
#Se requiere la variable res_dir
res_dir=../RESULTS
#Recuperar parametros
repetitions=$1
shift 1
threads=$@

echo "Compilando..."
make all
echo "Done"

mkdir perf
mkdir callgraphs
echo "Ejecutando Prueba DEV_NULL..."
for num_threads in $threads
do
	echo "Evaluando "$num_threads" Threads"
	linea="$num_threads,";
	for ((i=1 ; $i<=$repetitions ; i++))
	{
		perf record -g -- ./dev_null $num_threads > aux &
		#./dev_null $num_threads > aux &
		pid=$!
		sleep 1
		wait $pid
		linea="$linea$(cat aux)"
		rm aux
		perf_file="perf/{"$num_threads"}perf_"$i".data"
		output_perf_file="perf/{"$num_threads"}perf_"$i".txt"
		perf report > $output_perf_file

		callgraph_outputfile="callgraphs/{"$num_threads"}perf_"$i".png"
		perf script | python ../gprof2dot.py -f perf | dot -Tpng -o $callgraph_outputfile
		mv perf.data $perf_file
	}
	output_csv_file=$res_dir"/DEV_NULL_times.csv"
	echo "$linea" >> $output_csv_file
done
make clean
echo "Done"