#!/bin/bash
#Se requiere la variable res_dir
res_dir=../RESULTS
#Recuperar parametros
repetitions=$1
shift 1
threads=$@

echo "Prueba UDP"
cd UDP

echo "Compilando..."
make all
echo "Done"

#La Prueba UDP requiere diferencias NTHREADS de NSOCKETS
echo "Ejecutando Prueba..."
for num_threads in $threads
do
	echo "Evaluando "$num_threads" Threads"
	linea="$num_threads,";
	for ((i=1 ; $i<=$repetitions ; i++))
	{
		#perf record ./server $num_threads 1 > aux &
		./server $num_threads 1 > aux &
		pid=$!
		sleep 1
		./client 1 127.0.0.1 > /dev/null &
		pid2=$!
		sleep 1
		wait $pid
		wait $pid2
		linea="$linea$(cat aux)"
		rm aux
		#output_perf_file=$res_dir"/"$num_threads"perf_"$i".txt"
		#sudo perf report >> $output_perf_file
	}
	output_csv_file=$res_dir"/UDP_times.csv"
	echo "$linea" >> $output_csv_file
done
make clean
echo "Done"
cd ..