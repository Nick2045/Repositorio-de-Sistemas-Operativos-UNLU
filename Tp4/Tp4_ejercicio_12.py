import os
import subprocess
import math

"""
12. Construya un programa que simule la asignación de espacio en disco y manejo 
de directorio para una estructura de administración del tipo FAT. El programa 
deberá verificar si el espacio disponible en disco es suficiente para el archivo 
que se desea almacenar, deberá permitir almacenar, borrar un archivo, 
duplicación de nombre en un mismo archivo, permitir ver el contenido de la 
FAT simulada al igual que el directorio  (directorio de un solo nivel). 
"""

FILAS = 20

def limpiar_consola():
    cmd = 'cls' if os.name == 'nt' else 'clear'
    subprocess.run(cmd,shell=True)

def menu():
    limpiar_consola()
    print(
    """
    ------ Simulacion de Estructura de Administración del tipo FAT ------
    Seleccione la opcion [1] para CARGAR UN ARCHIVO.
    Seleccione la opcion [2] para BUSCAR Y MOSTRAR UN ARCHIVO.
    Seleccione la opcion [3] para LISTAR TABLAS FAT.
    Seleccione la opcion [4] para BORRAR UN ARCHIVO.
    Seleccione la opcion [0] para SALIR DE LA SIMULACION.
    """
    )

# Esta es la funcion para crear el archivo y el dato que quiero guardar en la tabla de datos
def crear_archivo(Tabla_de_Indices, Tabla_de_datos, Tabla_de_bits):
    print("\n=========== CARGAR UN ARCHIVO ===========")
    nombreDeArchivo = "" # Donde ira el nombre del archivo 
    datoNuevo = "" # el dato que ira en el archivo
    espacioQueOcuparaElDato = None # Para saber la longitud de lo que ocupara mi dato

    indiceInicialDeDato = None #Para actualizar mi tabla de indices al final
    extensionDelDato = None #Para actualizar mi tabla de indices al final

    indicesLibresParaDato = list() #Para llevar la cuenta de los indices libres donde ira mi dato
    trozosDeDato = list() # Aca va mi dato partido en trozo de 10 en 10 caracteres

    Bandera1 = False # False significa que no hay mas espacio para poner un nuevo archivo en el disco, True significa lo contrario
    for fila in Tabla_de_Indices: # Esto verificara que haya por lo menos un espacio libre en la tabla de indices para poner un archivo
        archivo = fila[0]
        indiceInicial = fila[1]
        extension = fila[2]
        borradoDisponible = fila[3]
    
        if (borradoDisponible == True) and (archivo == "") and (indiceInicial == -1) and (extension == -1):
            Bandera1 = True
            break
    
    if Bandera1 == False: # Si no hay lugar, se termina aca la carga y se explica el porque
        return False, "\n¡NO PUEDES CARGAR UN NUEVO ARCHIVO PORQUE YA NO HAY MAS ESPACIO EN EL DIRECTORIO PARA UNO NUEVO!"

    nombreDeArchivo = input("\nIngrese el nombre del nuevo archivo que quieres cargar: ") # Si hay espacio, le pido al usuario el nombre que le quiero poner al archivo
    while (len(nombreDeArchivo) > 20 or len(nombreDeArchivo) == 0): # Me aseguro que tenga menos de 20 caracteres o justo 20, si tiene mas le hago al usuario volver a poner otro nombre. Tambien me aseguro de que haya ingresado al menos algo
        print(f"\n¡El archivo '{nombreDeArchivo}' no puede tener mas de 20 caracteres o no puedo ingresar un nombre vacio!")
        nombreDeArchivo = input("\nIngrese de nuevo el nombre del nuevo archivo que quieres cargar: ")

    Bandera2 = False # false significa que no hay un archivo con el mismo nombre, si es True quiere decir que si lo hay. O sea, recorro la tabla de indices para fijarme que no haya ya un archivo con el mismo nombre que el nuevo que quiero poner
    for fila in Tabla_de_Indices:
        archivo = fila[0]
        borradoDisponible = fila[3]

        if (borradoDisponible == False) and (archivo == nombreDeArchivo):
            Bandera2 = True
            break

    if Bandera2 == True: #Si hay un archivo con el mismo nombre, se termina aca la carga porque no pueden existir 2 archivos con el mismo nombre
        return False, "\n¡NO PUEDES CARGAR UN NUEVO ARCHIVO CON EL MISMO NOMBRE DE UNO YA EXISTENTE EN EL DIRECTORIO!"

    datoNuevo = input("\nIngrese el dato que quieres guardar en este archivo: ")# Ingreso el dato que quiero meter en el archivo
    while (len(datoNuevo) == 0): # Me aseguro que haya ingresado algo
        print(f"\n¡El archivo '{datoNuevo}' no puede contener un dato vacio!")
        datoNuevo = input("\nIngrese de nuevo el dato que quieres poner en el archivo: ")
    espacioQueOcuparaElDato = math.ceil(len(datoNuevo)/10) # Me fijo cuantas secciones de la tabla de datos va a ocupar mi dato total, para eso lo divido por 10 a mi total de caracteres de mi dato y lo redondeo para arriba para sacar un entero

    Bandera3 = False # Si es False quiere decir que no hay suficientes espacios para el dato, si es True significa que si los hay. O sea, Recorro la tabla de indices para saber si hay las secciones que necesito para meter mi dato en la tabla de datos.
    for indice, libreOcupado in enumerate(Tabla_de_bits):
        estado = libreOcupado[0]
        if (estado == True) and (len(indicesLibresParaDato) < espacioQueOcuparaElDato): # Me fijo si el estado de esa secciones esta libre y si aun no alcance la cantidad de secciones libres que necesito
            indicesLibresParaDato.append(indice) # Si el estado es libre y aun me faltan secciones libres para mi tado, meto el indice de esa seccion libre en mi lista de indices libres para mi dato

        if (len(indicesLibresParaDato) == espacioQueOcuparaElDato): # Cuando alcanzo la totalidad de secciones libres necesarias en mi lista, termino el recorrido por la tabla de bits
            Bandera3 = True
            break

    if Bandera3 == False: # Si recorri toda la tabla de bits y no habia lugar, se termia aca y le dijo que no habia lugar para el dato que queria poner
        return False, "\n¡NO HAY SUFICIENTE ESPACIO PARA COLOCAR ESTE NUEVO ARCHIVO EN EL DISCO!"

    while (len(datoNuevo) > 0): # Si habia lugar, toca partir mi dato en trozo de 10 en 10 y a cada trozo lo guardo en mi lista de trozos de dato.
        texto = datoNuevo[:10] # Tomo los 10 primeros caracteres de mi dato
        trozosDeDato.append(texto) # guardo esos 10 caracteres en mi lista
        datoNuevo = datoNuevo[10:] # actualizo la variable de mi dato para solo tener en cuenta el string restante

    indiceInicialDeDato = indicesLibresParaDato[0] # guardo indice inicial del dato para actualizar mas tarde la tabla de indices
    extensionDelDato = len(indicesLibresParaDato)# guardo la extension de mi dato para actualizar mas tarde la tabla de indices

    contador = len(indicesLibresParaDato) # Creo un contador para saber cuantos bucles debo hacer para guardar todas las partes del dato
    while (contador > 0): # Esto solo funciona bajo la premisa de que la lista de indices libres y la lista de trozo de textos tiene la misma longitud, sino no funciona, pero si se llego hasta aca ambas deberian de tener la misma longitud
        contador = contador - 1 # Descuento 1 al contador
        indice = indicesLibresParaDato.pop(0) # Tomo indice de la lista de indices libres
        texto = trozosDeDato.pop(0) # Tomo trozo de dato de mi lista que almacena la totalidad de mi dato en trozos

        Tabla_de_bits[indice][0] = False # declaro que en ese indice de la tabla de bits estara ocupado
        Tabla_de_datos[indice][0] = texto # Coloco el dato en ese indice de la tabla de datos en

        if (len(indicesLibresParaDato) > 0) and (len(trozosDeDato) > 0):
            Tabla_de_datos[indice][1] = indicesLibresParaDato[0] # Si aun quedan por poner mas trozo, enlazo este trozo de dato con el siguiente para no perderme
        else:
            Tabla_de_datos[indice][1] = -1 # Si llegue al ultimo trozo y ya no hay ni mas indices que mirar no mas trozos que guardad, declaro que el siguiente de esta seccion es nulo
            
    for fila in Tabla_de_Indices: # Busco cualquier espacio libre de la tabla de indices para declarar que puse un dato.
        archivo = fila[0]
        indiceInicial = fila[1]
        extension = fila[2]
        borradoDisponible = fila[3]
        
        if (borradoDisponible == True) and (archivo == "") and (indiceInicial == -1) and (extension == -1):
            fila[0] = nombreDeArchivo
            fila[1] = indiceInicialDeDato
            fila[2] = extensionDelDato 
            fila[3] = False
            break

    return True, "\n¡El nuevo archivo fue cargado con exito!"

def buscar_y_mostrar_archivo(Tabla_de_Indices, Tabla_de_datos):
    print("\n=========== BUSCAR Y MONSTRAR UN ARCHIVO ===========")
    indiceDelArchivoBuscadoParaTablaDeIndices = None #Si encuentro el archivo en la tabla de indices, aca guardo el indice de su fila
    NombreOriginalDelArchivo = "" #Aca guardo su nombre directamente sacado de la tabla de indices
    indicieInicial = None #Aca guardare el indice del inicio del dato pero sin modificar
    extensionDeDato = None # Aca guardo la cantidad de secciones de ocupa el dato segun tabla de indices
    indiceActual = None # Aca guardo el indice donde estoy actualmente
    indiceSiguiente = None #Aca guardo el proximo indice a vistar o el final
    datoArmado = ""# Aca guardare el contedio del dato guardado en el archivo armado tras recorrer tabla de datos

    nombreDeArchivoBuscado = input("\nIngrese el nombre del archivo que buscas: ") # Se le pide al usuario el nombre del archivo que esta buscando
    while (len(nombreDeArchivoBuscado) > 20 or len(nombreDeArchivoBuscado) == 0): # Me aseguro que tenga menos de 20 caracteres o justo 20, si tiene mas le hago al usuario volver a poner otro nombre. Tambien me aseguro de que haya ingresado al menos algo
        print(f"\n¡El archivo '{nombreDeArchivoBuscado}' que buscas no puede tener mas de 20 caracteres o no puede estar vacio!")
        nombreDeArchivoBuscado = input("\nIngrese de nuevo el nombre del archivo que buscas: ")

    bandera1 = False
    for indice, fila in enumerate(Tabla_de_Indices): #Teniendo en cuenta el nombre ingresaod por el usuario, lo busco en la tabla de indices. Me voy fijando si el nombre coincide y si no esta borrado
        archivo = fila[0]
        borradoDisponible = fila[3]

        if (archivo == nombreDeArchivoBuscado) and (borradoDisponible == False): # Si lo encunetra, entonces marco que lo encontre y me guardo el indice de la fila donde esta
            bandera1 = True
            indiceDelArchivoBuscadoParaTablaDeIndices = indice
            break

    if (bandera1 == False): #Si no lo encontre aca termina el codigo
        return False, f"\n¡El archivo '{nombreDeArchivoBuscado}' buscado no se encuentra en este directorio!"

    NombreOriginalDelArchivo = Tabla_de_Indices[indiceDelArchivoBuscadoParaTablaDeIndices][0] # Guardo el nombre del archivo sacado directamente de la tabla de indices
    indicieInicial = Tabla_de_Indices[indiceDelArchivoBuscadoParaTablaDeIndices][1] #Me guardo el inidice inicial sin modificaciones
    extensionDeDato = Tabla_de_Indices[indiceDelArchivoBuscadoParaTablaDeIndices][2] #Me guardo la cantidad de secciones que ocupa el dato en la tabla de indices

    indiceActual = indicieInicial # En un inicio es una copia del indice inicial, pero se modificara a medida que recorra la tabla de datos
    datoArmado += Tabla_de_datos[indiceActual][0] # Aca ire guardando poco a poco los trozos de mi dato hasta armarlo por completo
    indiceSiguiente = Tabla_de_datos[indiceActual][1] # Aca se ira almacenando el indice de la proxima seccion a visitar para armar el dato o el fin de recorrido
    contador = 1 # Un contador que ira contando la cantidad de secciones recorridas, hasta este momento ya recorri una. Sera clave para comparar con 'extensionDelDato' cuando termine de recorrer la tabla de datos para saber si hay concistencia

    while indiceSiguiente != -1 and contador < extensionDeDato: # Si hay que seguir recorriendo la tabla de datos para armar el dato porque el dato ocupa mas de una seccion, pues se hace. Se verifica ,al termina cada seccion, si llegue al final del recorrido o si ya recorri la cantidad de secciones que debia recorrer declaradas en la tabla de indices
        indiceActual = indiceSiguiente
        datoArmado += Tabla_de_datos[indiceActual][0]
        indiceSiguiente = Tabla_de_datos[indiceActual][1]
        contador +=1

    if contador != extensionDeDato or indiceSiguiente != -1: # Si al terminar el recorrido, el indice siguiente no es -1 o si el contador no coincide con la extension del dato, hubo un error de inconsistencia
        return False, "\n OCURRIO UN ERROR DE INCONSISTENCIA ENTRE LA CANTIDAD DE SECCIONES RECORRIDAS Y LA CANTIDAD DE SECCIONES DECLARADAS EN LA TABLA DE INDICES. "    

    print(f"""\n
        ====================== Archivo '{NombreOriginalDelArchivo}' ======================
        Indice inicial: {indicieInicial} 
        Secciones ocupadas: {extensionDeDato} 
        Dato contenido en el archivo: {datoArmado}
          """)    

    return True, "\n¡El archivo fue encontrado y su contenido fue leido con exito!"
    
def listar_Tablas_FAT(Tabla_de_Indices, Tabla_de_datos, Tabla_de_bits): # ES OBVIO LO QUE SE HACE ACA, SOLO SE MUESTRA EL CONTENIDO DE CADA TABLA Y YA. 
    print("\n=========== LISTAR TABLAS FAT ===========")

    print("\n|======================= TABLA DE INDICES =======================|")
    print("|----------------------------------------------------------------|")
    print("| Nombre               | Bloque_Inicial | Cantidad | Borrado     |")
    print("|----------------------------------------------------------------|")

    for fila in Tabla_de_Indices: 
        archivo = fila[0]
        indiceInicial = fila[1]
        extension = fila[2]
        borradoDisponible = fila[3]

        print(f"| {archivo:<20} | {indiceInicial:^14} | {extension:^8} | {str(borradoDisponible):^11} |")
        
    print("|----------------------------------------------------------------|\n\n")

    print("|============= TABLA DE DATOS =============|")
    print("|------------------------------------------|")
    print("|Datos                 | Bloque_Siguiente  |")
    print("|------------------------------------------|")
    for fila in Tabla_de_datos: 
        dato = fila[0]
        siguiente = fila[1]
    
        print(f"| {dato:<20} | {siguiente:^16} |")

    print("|------------------------------------------|\n\n")

    print("|========== TABLA DE BITS ===========|")
    print("|----------------------------------|")
    print("| Posicion           | Estado      |")
    print("|----------------------------------|")
    for indice ,fila in enumerate(Tabla_de_bits): 
        ocupadoDisponible = fila[0]
    
        print(f"| {indice:^18} | {str(ocupadoDisponible):^11} |")

    print("|----------------------------------|\n\n")

def borrar_Archivo(Tabla_de_Indices, Tabla_de_datos, Tabla_de_bits):
    print("\n=========== BUSCAR Y BORRAR UN ARCHIVO ===========")
    indiceDelArchivoBuscadoParaTablaDeIndices = None #Aca guardo el indice del archivo que quiero borrar
    extensionDeDato = None # Aca la extension de mi dato para futuras verificaciones
    indiceActual = None #El indice actual 
    indiceSiguiente = None # El indice siguiente

    nombreDeArchivoBuscado = input("\nIngrese el nombre del archivo que buscas parar borrar: ") #De aca hasta la comporbacion de bandera es lo mismo que hay en mostrar solo que con unpar de print cambiados para reflejar que voy a buscar y borrar
    while (len(nombreDeArchivoBuscado) > 20 or len(nombreDeArchivoBuscado) == 0): 
        print(f"\n¡El archivo '{nombreDeArchivoBuscado}' que buscas no puede tener mas de 20 caracteres o no puede estar vacio!")
        nombreDeArchivoBuscado = input("\nIngrese de nuevo el nombre del archivo que buscas parar borrar: ")

    bandera1 = False
    for indice, fila in enumerate(Tabla_de_Indices): 
        archivo = fila[0]
        borradoDisponible = fila[3]

        if (archivo == nombreDeArchivoBuscado) and (borradoDisponible == False): 
            bandera1 = True
            indiceDelArchivoBuscadoParaTablaDeIndices = indice
            break

    if (bandera1 == False): 
        return False, f"\n¡El archivo '{nombreDeArchivoBuscado}' buscado no se encuentra en este directorio!"

    extensionDeDato = Tabla_de_Indices[indiceDelArchivoBuscadoParaTablaDeIndices][2] #Me guardo la extension de dato para verificar mas adelante los recorridos

    indiceActual = Tabla_de_Indices[indiceDelArchivoBuscadoParaTablaDeIndices][1] # Obtengo el indice inicial del dato
    indiceSiguiente = Tabla_de_datos[indiceActual][1] # Obtengo que indice del siguiente trozo del dato
    Tabla_de_datos[indiceActual][0] = "" #Borrro este trozo de dato
    Tabla_de_datos[indiceActual][1] = -1 # Le marco que ya no tiene siguiente porque el nodo lo vacie
    Tabla_de_bits[indiceActual][0] = True #Declaro en la tabla de bits que este espacio esta libre
    contador = 1 # Se lleva la cuenta de la seccion recorrida

    #Si hay que seguir recorriendo el dato, se sigue aca haciendo lo mismo de antes. #Se cometio el error en una pruba pasada de poner 'indiceSiguiente = Tabla_de_datos[indiceActual][1]' despues de ya haber borrado todo, lo que hace que cuando llegue a esta liena apunte ahora a -1, ya se soluciono
    while indiceSiguiente != -1 and contador < extensionDeDato: # Cuando borre todo el dato, ya siguiente deberia estar en -1 y el contador tendria que haber recorrido lo que se dijo que era la extension del dato
        indiceActual = indiceSiguiente
        indiceSiguiente = Tabla_de_datos[indiceActual][1]
        Tabla_de_datos[indiceActual][0] = ""
        Tabla_de_datos[indiceActual][1] = -1
        Tabla_de_bits[indiceActual][0] = True
        contador +=1

    if contador != extensionDeDato or indiceSiguiente != -1:# Se hace la verificacion del siguiente y del contador apra estar seguro.
        return False, "\n OCURRIO UN ERROR DE INCONSISTENCIA ENTRE LA CANTIDAD DE SECCIONES RECORRIDAS Y LA CANTIDAD DE SECCIONES DECLARADAS EN LA TABLA DE INDICES. "  

    #Una vez borrado el dato, solo queda ajustar la tabla de indices
    Tabla_de_Indices[indiceDelArchivoBuscadoParaTablaDeIndices][0] = ""
    Tabla_de_Indices[indiceDelArchivoBuscadoParaTablaDeIndices][1] = -1
    Tabla_de_Indices[indiceDelArchivoBuscadoParaTablaDeIndices][2] = -1
    Tabla_de_Indices[indiceDelArchivoBuscadoParaTablaDeIndices][3] = True

    return True, "\n¡El archivo fue encontrado y su contenido fue borrado con exito!"       
     

    

def leer_entero(Opcion, minimo, maximo):
    entero = None
    while (True):
        try:
            entero = int(input(Opcion))
            if (entero >= minimo) and (entero <= maximo):
                break
            else:
                print("!Error¡ numero fuera de rango. Intentelo de nuevo.")
        except ValueError:
            print("!Error¡ el valor ingresado es invalido. Intentelo de nuevo.")

    return entero

def  main():
    eleccion = -1
    Tabla_de_Indices = [["",-1,-1,True] for _ in range(FILAS)]
    Tabla_de_datos = [["",-1] for _ in range(FILAS)]
    Tabla_de_bits = [[True] for _ in range(FILAS)]

    while(eleccion != 0):
        menu()
        eleccion = leer_entero("\nOpcion elegida: ", 0, 4)

        match eleccion:
            case 0:
                print("\nSaliendo de la simulacion...")
            case 1:
                resultadoDeCarga, Mensaje  = crear_archivo(Tabla_de_Indices,Tabla_de_datos,Tabla_de_bits)

                if resultadoDeCarga == True:
                    print(Mensaje)
                else:
                    print(Mensaje)

                input("\nPresione Enter para continuar...")
            case 2: 
                resultadoDeMostrar, Mensaje2  = buscar_y_mostrar_archivo(Tabla_de_Indices,Tabla_de_datos) 
                
                if resultadoDeMostrar == True:
                    print(Mensaje2)
                else:
                    print(Mensaje2)

                input("\nPresione Enter para continuar...")
            case 3:

                listar_Tablas_FAT(Tabla_de_Indices, Tabla_de_datos, Tabla_de_bits)

                input("\nPresione Enter para continuar...")
            case 4:
                resultadoDeBorrar, Mensaje3  = borrar_Archivo(Tabla_de_Indices, Tabla_de_datos, Tabla_de_bits) 
                                
                if resultadoDeBorrar == True:
                    print(Mensaje3)
                else:
                    print(Mensaje3)

                input("\nPresione Enter para continuar...")
            case _:
                print("\nOpcion no valida. Cerradno programa por precaucion....")
                eleccion = 0
                input("\nPresione Enter para continuar...")

    print()        
    input("\nPresione Enter para continuar...")


if __name__ == '__main__':
    main()