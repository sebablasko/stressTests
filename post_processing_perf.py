import os
import glob

#pido archivos del directorio actual
files = glob.glob(os.getcwd()+"/perf/*perf*.txt")

#estructuras para almacenar la informacion
spinlocks = {}
muestras = 0
threads_probados = []

#recuperar threads probados y numero de muestras
for file in sorted(files):
	archivo = open(file, 'r')
	fileName = os.path.basename(archivo.name)
	muestras = max(muestras, int(fileName[fileName.find("_")+1 : fileName.find(".")]))
	threads = int(fileName[fileName.find("{")+1 : fileName.find("}")])
	if threads not in threads_probados:
		threads_probados.append(threads)


def crearDict(threads_probados, muestras):
	a = {}
	for t in threads_probados:
		a[t] = [0]*muestras
	return a

#abrir cada archivo con el formato dado
for thread in threads_probados:
	for i in range(muestras):
		nombre = "{"+str(thread)+"}perf_"+str(i+1)+".txt"
		ruta = os.getcwd()+"/perf/"+nombre
		archivo = open(ruta, 'r')
		for line in archivo:
			pos = line.find("spin")
			if(pos>0):
				porcentaje = line[:line.find("%")].split()[0]
				funcion = line[line.find("[k]")+3:-1].split()[0]
				if not funcion in spinlocks:
					spinlocks[funcion] = crearDict(threads_probados, muestras)
				spinlocks[funcion][thread][i] = porcentaje

		archivo.close()

#Construir planilla con resultados
salida = open("perfTests.csv", "w+")
for spin_function in sorted(spinlocks):
	salida.write(spin_function + "\n")
	for t in sorted(spinlocks[spin_function]):
		salida.write(str(t) + ",")
		for valor in spinlocks[spin_function][t]:
			salida.write(str(valor) + ",")
		salida.write("\n")
	salida.write("\n\n\n")
salida.close()