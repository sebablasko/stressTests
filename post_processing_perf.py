import os
import glob

#variables para almacenar la informacion
lock_functions = {}				# Diccionario para funciones de llamada a spinlocks. clave: nombre funcion; valor: lista con muestras de % de llamadas
muestras = 0					# variable para determinar cuantas muestras (repeticiones) se hizo con cada configuración de threads
threads_probados = []			# lista para almacenar cantidad de threads en prueba

# Función para crear un diccionario para una función de spinlock, en el cual almacenar muestras de % de llamadas segun numero de threads.
# retorna diccionario
#	clave: cantidad de threads
# 	valor: lista con % de llamadas (han de haber muestras valores)
def crearDict(threads_probados, muestras):
	a = {}
	for t in threads_probados:
		a[t] = [0]*muestras
	return a

# 1.- Recuperar threads_probados y muestras
files = glob.glob(os.getcwd()+"/perf/*perf*.txt")
for file in sorted(files):
	archivo = open(file, 'r')
	fileName = os.path.basename(archivo.name)
	muestras = max(muestras, int(fileName[fileName.find("_")+1 : fileName.find(".")]))
	threads = int(fileName[fileName.find("{")+1 : fileName.find("}")])
	if threads not in threads_probados:
		threads_probados.append(threads)

# 2.- Abrir cada archivo de datos según la nomenclatura de su nombre
for thread in threads_probados:
	for i in range(muestras):
		nombre = "{"+str(thread)+"}perf_"+str(i+1)+".txt"
		ruta = os.getcwd()+"/perf/"+nombre

		# 3.- Para cada archivo, buscar las apariciones de funciones pedidas según aparición de palabra en su nombre
		archivo = open(ruta, 'r')
		for line in archivo:
			pos = line.find("spin")
			if(pos>0):
				porcentaje = line[:line.find("%")].split()[0]
				funcion = line[line.find("[k]")+3:-1].split()[0]
				if not funcion in lock_functions:
					lock_functions[funcion] = crearDict(threads_probados, muestras)
				lock_functions[funcion][thread][i] = porcentaje
		archivo.close()


# 4.- Generar csv con resultados
salida = open("perfTests.csv", "w+")
for spin_function in sorted(lock_functions):
	salida.write(spin_function + "\n")
	for t in sorted(lock_functions[spin_function]):
		salida.write(str(t) + ",")
		for valor in lock_functions[spin_function][t]:
			salida.write(str(valor) + ",")
		salida.write("\n")
	salida.write("\n\n\n")
salida.close()