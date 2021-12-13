En el presente trabajo se implementa un TDA Heap y todas las funciones necesarias para compilar y ejecutar correctamente el juego de consola pedido en el enunciado. Específicamente se hace entrega de un archivo makefile, de los archivos de los TDAs ABB y Lista previamente hechos, los archivos del TDA Heap, un archivo juego.c con la función Main, varios archivos auxiliares (logica.c, menu.c, gimnasios.c, personaje.c, y batallas.c), y todos sus respectivos headers.

ESTRUCTURAS
	Entrenadores en un gimnasio:
		Para almacenar a los entrenadores dentro de un gimnasio la estructura utilizada es una pila. Se elige esta estructura ya que hay que luchar contra los entrendores en orden inverso al que aparecen en el archivo: el último entrenador del archivo será el primero contra el que hay que luchar. Además, una vez que se derrota a un entrenador ya no se deberá luchar nuevamente contra él, por lo que no se requiere usar una lista. 

	Pokemones de un entrenador: 
		En el trabajo, la estructura que se usa es una lista. Los pokemones del entrenador deben ser accedidos en el mismo orden en que fueron agregados, como en una cola, pero con la condición de que si se pierde la batalla habrá que empezar nuevamente desde el principio. Por esta razón se prefiere utilizar una lista, ya que puede ser recorrida secuencialmente y de manera sencilla con un iterador externo, y también tiene la flexibilidad para reiniciar el recorrido en cualquier momento, con solo crear un nuevo iterador. 

	Pokemones obtenidos por el personaje: 
		Para almacenar este conjunto de Pokemones se utiliza un Árbol Binario de Búsqueda. Se elige este TDA ya que hay que buscar dentro de la estructura a la hora de seleccionar los Pokemones de combate. Un ABB, intrínsecamente, permite hacer una búsqueda binaria de sus elementos, que es mas rápida que la búsqueda lineal ofrecida por una lista, donde se recorren los elementos uno a uno hasta el final.

	Pokemones de combate del personaje:
		Las razones por las que se elige una lista para los Pokemones de combate son las mismas que para los Pokemones de un entrenador. Hay que poder recorrer los elementos secuencialemnte como fueron agregados, pero comenzando desde el principio varias veces, tanto para mostrar a los Pokemones como para reiniciar una batalla. Otra buena opción para este caso hubiera sido un vector dinámico, ya que sabemos que como máximo habrá 6 elementos y hay que cambiar a los Pokemones muy seguido. 

MODIFICACIONES A TDAS
	Los únicos cambios realizados a los TDAs ya implementados son:
	- El agregado de la función arbol_elementos(abb_t* arbol) al ABB, que devuelve la cantidad de elementos que hay en el árbol.
	- La modificación de la función lista_destruir(lista_t* lista, void (*destructor)(void*)) para que reciba ese segundo parámetro: un puntero a una función de destrucción que si no es NULL se llama sobre los elementos de la lista antes de eliminar los nodos. 

EJECUCIÓN
	Para compilar el programa con GCC hay que correr la linea:
		make compilar
	Para compilar y luego correr con Valgrind:
		make valgrind
	Para compilar y luego correr:
		make
