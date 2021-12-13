En el presente trabajo se implementan las operaciones de un TDA ABB (Árbol Binario de Búsqueda), así como también las pruebas para garantizar su correcto funcionamiento. En particular, se hace entrega de un archivo abb.c con el código de las funciones, un framework de pruebas pa2mm.h modificado, un archivo de pruebas llamado pruebas.c, y un makefile. 

EJECUCIÓN
	Para compilar el programa con GCC:
		make compilar
	Para compilar y luego correr con Valgrind:
		make valgrind
	Para compilar y luego correr:
		make

FUNCIONAMIENTO
	La implementación elegida para las funciones del TDA es en todos los casos recursiva. A continuación se detalla el funcionamiento de algunas funciones:

	int arbol_borrar(abb_t* arbol, void* elemento)
		Verifica que el árbol recibido exista y no esté vacío. En ese caso, crea la variable auxiliar "encontrado", y llama a arbol_borrar_recursivo.

	nodo_abb_t* arbol_borrar_recursivo(nodo_abb_t* nodo_actual, void* elemento, abb_comparador comparador, abb_liberar_elemento destructor, int* encontrado)
		La función recorre el árbol recursivamente buscando el elemento a borrar. Para esto utiliza el comparador recibido por parámetro. Si encuentra el nodo, maneja los 3 casos posibles: el nodo no tiene hijos, el nodo tiene un hijo, o el nodo tiene 2 hijos. En el primer caso se destruye el elemento con el destructor recibido, se libera el nodo, y se devuelve NULL para que el nodo padre deje de apuntar al nodo destruído. 
		En el segundo caso se hace lo mismo, pero en lugar de devolver NULL, se devuelve el puntero al nodo hijo, de manera que el padre ahora apunte a él.
		Finalmente, cuando el nodo tiene 2 hijos, lo que se hace es destruir el elemento y reemplazarlo con el mayor elemento del sub-árbol izquierdo. Para lograr esto se llama a la funcion sacar_nodo_maximo_recursivo, se guarda el nuevo elemento en el nodo actual, y se lo devuelve, ya que no fué liberado. 

	nodo_abb_t* sacar_nodo_maximo_recursivo(nodo_abb_t* nodo_actual, void** elemento_maximo)
		Se recorre recursivamente siempre hacia el nodo derecho hasta llegar a NULL. De esa manera encontramos el máximo elemento del árbol. Luego se guarda el elemento en la variable recibida y se elimina el nodo.

Un árbol binario es una estructura de datos no lineal. Se define recursivamente como un conjunto finito de nodos que puede estar vacío, o contener un nodo "raiz" y otros dos árboles binarios, llamados "sub-árbol izquierdo" y "sub-árbol derecho" de la raiz. La implementación mas usada, y la elegida para este trabajo, es una en la que los nodos son estructuras dinámicas que contienen al elemento que almacenan, y punteros a sus nodos "hijos" izquierdo y derecho.
Una aplicación importante de los árboles binarios es su uso en la búsqueda. Si cada nodo del árbol contiene un valor "clave", y todos los elementos pueden ordenarse según esa clave, la restricción que convierte a un árbol binario en un ABB (Árbol Binario de Búsqueda) es la siguiente: para cada nodo X del árbol, el valor clave de todos los nodos del sub-árbol izquierdo es menor a la clave de X, y el valor clave es mayor en todos los nodos del sub-árbol derecho.
De esta manera es posible buscar un elemento del árbol como si se tratara de una búsqueda binaria, comparando con el nodo raíz y descendiendo hacia la izquierda o derecha según corresponda. 

El TDA ABB presentado en este trabajo utiliza memoria dinámica para almacenar los nodos, la que debe ser liberada antes de finalizar la ejecución del programa. Por otro lado, el usuario del TDA desconoce su funcionamiento interno, y es correcto que la implementación se mantenga ajena a él. Por esta razón es necesario tener una función de destrucción, para permitir al usuario liberar la memoria reservada sin necesidad de hacerlo manualmente, evitando que acceda a la estructura interna. También se requiere que al crear el TDA el usuario especifique, solo si es necesario, una función de destrucción de los elementos. Esto se hace ya que si se elimina un elemento o se destruye un árbol lleno, los punteros a los elementos se perderían, y por ende la memoria que ocupan no podría ser liberada. 

Las operaciones básicas del ABB (insertar, eliminar, buscar) llevan un tiempo proporcional a la altura del árbol, que es la longitud del camino mas largo desde la raiz hasta un nodo hoja. La altura promedio de un árbol binario de n nodos es O(log n), por lo que esa esa la complejidad algorítmica de las operaciones. Por otro lado, si se insertaran elementos en orden estrictamente creciente (o decreciente), el árbol "degeneraría" en una lista, haciendo que las operaciones sean O(n). Algunos tipos de árboles garantizan que se mantenga balanceado, para que las operaciones sigan siendo O(log n). 
Finalmente las operaciones de recorrido, al tener que visitar todos los nodos, tienen una complejidad de O(n). Y las operaciones simples como raíz, vacío, o crear, son constantes. 
