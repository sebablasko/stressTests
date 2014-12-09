#!/bin/bash
echo "Creando archivo FIFO pipe..."
mkfifo test_pipe
echo "Done"

echo "Compilando..."
make all
echo "Done"

echo "Ejecutando Prueba..."
#TODO: invocar prueba
echo "Done"

echo "Eliminando FIFO..."
rm test_pipe
echo "Done"