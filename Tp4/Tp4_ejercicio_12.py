import os
import subprocess

"""
12. Construya un programa que simule la asignación de espacio en disco y manejo 
de directorio para una estructura de administración del tipo FAT. El programa 
deberá verificar si el espacio disponible en disco es suficiente para el archivo 
que se desea almacenar, deberá permitir almacenar, borrar un archivo, 
duplicación de nombre en un mismo archivo, permitir ver el contenido de la 
FAT simulada al igual que el directorio  (directorio de un solo nivel). 
"""

FILAS = 100

def limpiar_consola():
    cmd = 'cls' if os.name == 'nt' else 'clear'
    subprocess.run(cmd,shell=True)

def menu():
    limpiar_consola()
    print(
    """
    ------ Simulacion de Estructura de Administración del tipo FAT ------
    Seleccione la opcion [1] para CARGAR UN ARCHIVO.
    Seleccione la opcion [2] para MOSTRAR UN ARCHIVO.
    Seleccione la opcion [3] para LISTAR TODOS LOS ARCHIVOS CARGADOS.
    Seleccione la opcion [4] para BORRAR UN ARCHIVO.
    Seleccione la opcion [0] para SALIR DE LA SIMULACION.
    """
    )

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
                print("\nOpcion 1: CARGAR UN ARCHIVO (No disponible)")
                input("\nPresione Enter para continuar...")
            case 2: 
                print("\nOpcion 2: MOSTRAR UN ARCHIVO (No disponible)")
                input("\nPresione Enter para continuar...")
            case 3:
                print("\nOpcion 3: LISTAR TODOS LOS ARCHIVOS CARGADOS (No disponible)")
                input("\nPresione Enter para continuar...")
            case 4:
                print("\nOpcion 4: BORRAR UN ARCHIVO (No disponible)")
                input("\nPresione Enter para continuar...")
            case _:
                print("\nOpcion no valida. Cerradno programa por precaucion....")
                eleccion = 0
                input("\nPresione Enter para continuar...")

    print()        
    input("\nPresione Enter para continuar...")



            
if __name__ == '__main__':
    main()