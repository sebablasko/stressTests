#!/bin/bash

for serverThreads in 1 2 4 8 16 32
do
	linea=$serverThreads";"
	for clients in 1 2 3 4 5 6 7 8 9 10
	do
		echo $serverThreads" threads en servidor"
		echo $clients" clientes corriendo"

		repetitions=6
		for ((i=1 ; $i<=$repetitions ; i++))
		{
			./server $serverThreads 1 >> aux &
			pid=$!
			sleep 1

			for ((j=1 ; $j<=$clients ; j++))
			{
				./client 1 127.0.0.1 > /dev/null &
			}
			wait $pid
			linea="$linea$(cat aux)"
			rm aux
		}
		linea="$linea;"
	done
	echo "$linea" >> server_client_relation.csv
done