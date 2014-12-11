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
echo "Done"


echo ""
echo "Iniciando pruebas..."

echo ""
echo "Prueba DEV_NULL"
cd DEV_NULL
./run.sh 2 1 2 3
cd ..

echo ""
echo "Prueba DEV_URANDOM"
cd DEV_URANDOM
./run.sh 2 1 2 3
cd ..

echo ""
echo "Prueba FIFO"
cd FIFO
./run.sh 2 1 2 3
cd ..

echo ""
echo "Prueba TCP"
cd TCP
./run.sh 2 1 2 3
cd ..

echo ""
echo "Prueba UNIX"
cd UNIX
./run.sh 2 1 2 3
cd ..

echo ""
echo "Prueba UDP"
cd UDP
./run.sh 2 1 2 3
cd ..