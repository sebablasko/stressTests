import os
import glob

#pido archivos del directorio actual
files = glob.glob(os.getcwd()+"/perf/*perf*.txt")
print os.getcwd()

#diccionarios para almacenar la informacion
spinlocks = {}

for file in files:
	archivo = open(file, 'r')
	threads = os.path.basename(archivo.name)
	threads = int(threads[threads.find("{")+1 : threads.find("}")])
	for line in archivo:
		pos = line.find("spin")
		if(pos>0):
			porcentaje = line[:line.find("%")].split()[0]
			funcion = line[line.find("[k]")+3:-1].split()[0]
			if not funcion in spinlocks:
				spinlocks[funcion] = {}
			if not threads in spinlocks[funcion]:
				spinlocks[funcion][threads] = []
			spinlocks[funcion][threads] = spinlocks[funcion][threads] + [porcentaje]
	archivo.close()

salida = open("perf.csv", "w+")
for key in sorted(spinlocks):
	salida.write(key + "\n")
	for t in sorted(spinlocks[key]):
		salida.write(str(t) + ",")
		for val in spinlocks[key][t]:
			salida.write(val + ",")
		salida.write("\n")
	salida.write("\n\n\n")
salida.close()