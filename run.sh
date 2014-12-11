#!/bin/bash
res_dir=RESULTS
cpu_info_file=CPUData.txt

mkdir $res_dir
echo "Guardar datos del Equipo..."
cpu_file=$res_dir"/"cpu_info_file
echo "Datos Kernel:" > $cpu_file
uname -a >> $cpu_file
cat /proc/version >> $cpu_file
cat /etc/*release >> $cpu_file
echo "" >> $cpu_file
echo "" >> $cpu_file
echo "Datos CPUs:" >> $cpu_file
lscpu >> $cpu_file
echo "" >> $cpu_file
echo "" >> $cpu_file
echo "Datos Memoria Ram" >> $cpu_file
free -t -m >> $cpu_file
echo "" >> $cpu_file
echo "Done\n"


echo "Iniciando pruebas..."

echo "Prueba DEV"
cd DEV
./DEV/run.sh 5 1 2 3 4 5
cd ..

echo "Prueba FIFO"
cd FIFO
./FIFO/run.sh 5 1 2 3 4 5
cd ..

echo "Prueba UDP"
cd UDP
./UDP/run.sh 5 1 2 3 4 5
cd ..

echo "Prueba TCP"
cd TCP
./TCP/run.sh 5 1 2 3 4 5
cd ..

echo "Prueba UNIX"
cd UNIX
./UNIX/run.sh 5 1 2 3 4 5
cd ..