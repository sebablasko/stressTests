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
#La Prueba UDP requiere diferencias NTHREADS de NSOCKETS
echo "Ejecutando Prueba..."
for num_threads in $threads
do
	echo "Evaluando "$num_threads" Threads"
	linea="$num_threads,";
	for ((i=1 ; $i<=$repetitions ; i++))
	{
		perf record -g -- ./server $num_threads 1 > aux &
		#./server $num_threads 1 > aux &
		pid=$!
		sleep 1
		./client 1 127.0.0.1 > /dev/null &
		pid2=$!
		./client 1 127.0.0.1 > /dev/null &
		pid3=$!
		./client 1 127.0.0.1 > /dev/null &
		pid4=$!
		./client 1 127.0.0.1 > /dev/null &
		pid5=$!
		sleep 1
		wait $pid
		#wait $pid2
		#wait $pid3
		#wait $pid4
		#wait $pid5
		linea="$linea$(cat aux)"
		rm aux
		perf_file="perf/{"$num_threads"}perf_"$i".data"
		output_perf_file="perf/{"$num_threads"}perf_"$i".txt"
		perf report > $output_perf_file

		callgraph_outputfile="callgraphs/{"$num_threads"}perf_"$i".png"
		perf script | python ../gprof2dot.py -f perf | dot -Tpng -o $callgraph_outputfile
		mv perf.data $perf_file
	}
	output_csv_file=$res_dir"/UDP_times.csv"
	echo "$linea" >> $output_csv_file
done
make clean
echo "Done"