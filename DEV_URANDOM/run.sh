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

#mkdir perf
echo "Ejecutando Prueba DEV_URANDOM..."
for num_threads in $threads
do
	echo "Evaluando "$num_threads" Threads"
	linea="$num_threads,";
	for ((i=1 ; $i<=$repetitions ; i++))
	{
		#perf record ./dev_urandom $num_threads > aux &
		./dev_urandom $num_threads > aux &
		pid=$!
		sleep 1
		wait $pid
		linea="$linea$(cat aux)"
		rm aux
		#output_perf_file=$res_dir"/perf/"$num_threads"perf_"$i".txt"
		#sudo perf report >> $output_perf_file
	}
	output_csv_file=$res_dir"/DEV_URANDOM_times.csv"
	echo "$linea" >> $output_csv_file
done
make clean
echo "Done"