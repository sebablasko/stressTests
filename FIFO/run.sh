#!/bin/bash
#Se requiere la variable res_dir
res_dir=../RESULTS
#Recuperar parametros
repetitions=$1
shift 1
threads=$@

echo "Creando archivo FIFO pipe..."
mkfifo test_pipe
echo "Done"

echo "Compilando..."
make all
echo "Done"

mkdir perf
echo "Ejecutando Prueba..."
for num_threads in $threads
do
	echo "Evaluando "$num_threads" Threads"
	linea="$num_threads,";
	for ((i=1 ; $i<=$repetitions ; i++))
	{
		perf record -- ./server $num_threads > aux &
		#./server $num_threads > aux &
		pid=$!
		sleep 1
		./client > /dev/null &
		./client > /dev/null &
		./client > /dev/null &
		./client > /dev/null &
		#pid2=$!
		sleep 1
		wait $pid
		wait $pid2
		linea="$linea$(cat aux)"
		rm aux
		perf_file="perf/{"$num_threads"}perf_"$i".data"
		output_perf_file="perf/{"$num_threads"}perf_"$i".txt"
		perf report > $output_perf_file
		mv perf.data $perf_file
	}
	output_csv_file=$res_dir"/FIFO_times.csv"
	echo "$linea" >> $output_csv_file
done
make clean
echo "Done"

echo "Eliminando FIFO..."
rm test_pipe
echo "Done"