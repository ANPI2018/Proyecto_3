-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
Proyecto 3

    Juan Pablo Brenes Coto
    Pablo Bustamante Mora
    Emily Sancho Murillo
    
Profesor: Dr. Pablo Alvarado Moya
Análisis numérico para ingeniería                12 de noviembre, 2018
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------

-------------------------------------------------------------------------------
                         REQUERIMIENTOS
-------------------------------------------------------------------------------

El programa hace uso del API OpengGL para la graficación de los
resultados.

  > sudo apt install libglu1-mesa-dev freeglut3-dev mesa-common-dev
  
Para la generación de la documentación del código, se requiere la
herramienta Doxygen

  > sudo apt install doxygen
  
  
-------------------------------------------------------------------------------
                    INSTRUCCIONES DE COMPILACIÓN
-------------------------------------------------------------------------------

Crear el directorio build:

  > mkdir build

Acceder al directorio:

  > cd build

Compilar el programa:

  > cmake ../ -DCMAKE_BUILD_TYPE=Release
  
  Si se desea generar la documentación en html del código, se debe
   compilar el programa con la opción "-DBUILD_DOC=ON"

    > cmake ../ -DBUILD_DOC=ON -DCMAKE_BUILD_TYPE=Release
  
  La documentación se genera en el directorio "doc", ubicado
   en el directorio raíz del programa.

Compilar

  > make
  
  
-------------------------------------------------------------------------------
                    INSTRUCCIONES DE EJECUCIÓN
-------------------------------------------------------------------------------

El ejecutable del programa se crea en el directorio 'bin', ubicado
dentro del directorio 'build' utilizado para la compilación del programa

Las opciónes aceptadas por el programa son:

  -t <valor> | <lista valores>   Indica temperatura(s) en borde superior
  -b <valor> | <lista valores>   Indica temperatura(s) en borde inferior
  -l <valor> | <lista valores>   Indica temperatura(s) en borde izquierdo
  -d <valor> | <lista valores>   Indica temperatura(s) en borde derecho
  
  -i [tblr] Aisla los bordes indicados (t=arriba, b=abajo, l=izquierda, r=derecha)

  -p <ruta> Indica el nombre del archivo con perfil térmico
  
  -h <valor> Número de pı́xeles horizontales en la solución
  -v <valor> Número de pı́xeles verticales en la solución
  
  -q Desactiva toda forma de visualización
  -f Activa el cálculo de flujo de calor.
  -g <valor> Tamaño de rejilla de visualización para flujo de calor
  -k <valor> Conductividad termica del material
  -m Permite medir el tiempo de ejecución (Solo muestra el tiempo de ejecución)
   
Ejemplos:

  > ./proyecto3 -k 10 -h 512 -v 512 -t 30 -b 100
  
  > ./proyecto3 -k 15 -h 128 -v 128 -l 100 25 40 -t 40 100
  
  > ./proyecto3 -k 5 -h 512 -v 512 -d 20 50 30 -b 10 40 90 20 -m
  
  > ./proyecto3 -k -h 1024 -v 1024 -p home/user/documents/profile.txt
  
  
Archivo con perfil térmico:

  Las temperaturas en los bordes pueden especificarse tambien por medio
  de un archivo de texto, que debe tener la siguiente estructura:
  
    <borde> = valor [valores*]
    <borde> = valor [valores*]
      
  Ejemplo:
    
    top = 50
    bottom = 30 40
    left = 10
    right = 10 30 50 70
      
  
Notas:
  * Las opciones '-k', '-h' y '-v' son siempre requeridas para la ejecución
    del programa.
    
  * Si alguna de las temperaturas en los bordes no se especifica en terminal, ni
    por medio del archivo con perfil térmico, se consideran dichos bordes
    como aislados.
    
  * Si se especifica por medio de la terminal el perfil térmico de alguno
    de los bordes dentro del archivo con perfil térmico, tendra prioridad lo especificado
    por medio de la terminal.
    
  * Por defecto la opción '-q' se encuentra activa, por lo que siempre mostrara
    el gráfico de distribución de temperatura.
    
  * Si se especifica la opción '-m', se ignora la opción '-q' y solo se 
    imprimira en consola el tiempo de ejecución.