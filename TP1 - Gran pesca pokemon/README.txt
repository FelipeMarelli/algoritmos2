FUNCIONAMIENTO
	bool procesar_linea(FILE* Arch, pokemon_t* pokemon_p);
		Función auxiliar utilizada por crear_arrecife(), recibe un puntero a archivo y un pokemon_t por referencia. Define un buffer de caracteres de tamaño BUFFER_SIZE y lo usa para leer con fgets. Si se leyó EOF, o el anteúltimo caracter del buffer no es un '\n' (pasaría si una linea es mas grande que el buffer), devuelve false. Si eso no pasa, usa sscanf para interpretar el buffer y llenar el pokemon_t que recibió como parámetro. La máscara usada "%100[a-zA-Z ];%5[0-9];%5[0-9];%50[a-zA-Z ]\n" lee los números como si fueran un string, permitiendo poner un límite a la cantidad de cifras, y los guarda en variables auxiliares (velocidad y peso) para mas adelante convertirlos a int con atoi. Usa el valor devuelto por sscanf para verificar que se haya leido correctamente todos los campos. Si todo sale bien, devuelve true.

	arrecife_t* crear_arrecife(const char* ruta_archivo);
		Intenta abrir el archivo con fopen, si esto falla devuelve NULL. Luego reserva espacio con malloc para un arrecife_t en el heap, verificando el resultado, e inicializa arrecife_p->pokemon a NULL. Este último paso es importante para que realloc actue como si fuera malloc la primera vez que use el puntero. Despues de eso se entra a un while cuya condición es que la función procesar_linea devuelva true (que además llena el pokemon_aux). Dentro del while se usa realloc para agrandar el vector apuntado por arrecife_p->pokemon y agregar el pokemon_aux. Si realloc falla se libera la memoria y se devuelve NULL. Si al finalizar el while arrecife->pokemon sigue siendo NULL se devuelve error nuevamente. Si todo sale ben, se devuelve el puntero a arrecife_p.

	bool verificar_trasladables(arrecife_t* arrecife, bool (*seleccionar)(pokemon_t*), int cantidad, int* indexo_trasladables);
		Función auxiliar utilizada por trasladar_pokemon. Verifica que haya suficientes pokemones trasladables (que devuelvan true al ser pasados a la función seleccionar) en el arrecife, y por cada uno guarda su indexo en el vector indexo_trasladables. Si no hay suficientes devuelve false, caso contrario devuelve true.

	int trasladar_pokemon(arrecife_t* arrecife, acuario_t* acuario, bool (*seleccionar_pokemon)(pokemon_t*), int cant_seleccion)
		Llama a la función verificar_trasladables y chequea que devuelva true. Para el traslado, se crea un nuevo vector en el heap con malloc que reemplazará a arrecife.pokemon, y se agranda acuario.pokemon con realloc. Intentar achicar el arrecife con realloc traía problemas con los cambios de indexos. Luego se hace un for loop por todos los pokemones en el arrecife. Si el indexo de un pokemon se encuentra en el vector indexo_trasladables, este se agrega al acuario. De lo contrario se agrega al nuevo arrecife. Al final se libera la memoria del arrecife viejo, y se reemplaza con el puntero al nuevo arrecife. 

EJECUCIÓN
	Para compilar el programa con GCC hay que correr la linea:
		make compilar
	Para compilar y luego correr con Valgrind:
		make valgrind
	Para compilar y luego correr:
		make


Un puntero es, por definición, una variable que almacena la posición de memoria de otra variable. Si se piensa a la
memoria de la computadora como un arreglo de celdas, donde cada una almacena un byte de información, un puntero podría ser el indexo de una de esas celdas. Es decir, un número que representa una posición específica en ese array. 
Cada puntero tiene asociado, además de un nombre y una dirección, un tipo de variable a la que este apunta: todos los punteros ocupan la misma cantidad de bytes entre sí, pero pueden apuntar a objetos de distinto tamaño en la memoria. Esto se especifica al declarar el puntero, de la siguiente manera: 
	int* puntero_a_int;
	char* puntero_a_char;
Un caso particular de esto es el puntero a void, o puntero comodín:
	void* puntero_a_void;
que significa al compilador que el puntero no tiene un tipo asociado al momento de ser declarado. Estos punteros se pueden asignar nomalmente, pero para ciertas operaciones es necesario hacer explícito el tipo de variable a la que apuntan, mediante un casteo (un cambio de tipo). Los casteos se realizan de las siguiente manera:
	int* puntero_a_int = (int*)malloc(sizeof(int));
donde el nuevo tipo de la variable se pone entre paréntesis.
Los principales operadores asociados a punteros son los de dirección e indirección. El primero devuelve un puntero a la dirección de memoria de una variable dada, y se denota con el símbolo & de la siguiente manera:
	int* A = &B;
se dice que ahora la variable A "apunta" a la variable B.
En cambio el operador de indirección o desreferencia, denotado con *, devuelve el contenido de la posición apuntada por un puntero. Si se piensa a un puntero como una flecha, se puede decir que este operador "recorre" la flecha. Un ejemplo de su uso podría ser:
	*A = 5;
donde se modifica el valor apuntado por A. 

También es posible sumar o restar un número a un puntero, modificando la dirección a la que apunta. Estas operaciones que se dan entre un puntero y un entero conforman la llamada "aritmética de punteros". Sea k un entero y ptr un puntero a double, entonces (ptr + k) es un puntero que apunta "k doubles más adelante que ptr". Es decir, no se suman k bytes a la dirección, sino k elementos del tipo al que apunta el puntero. Los bytes agregados a la dirección serían k * sizeof(double). 
De esta manera se explica la estrecha relación entre punteros y notación de arreglos. El nombre de un arreglo no es más que un puntero a la primera posición del mismo, entonces la expresión arreglo[10] es equivalente a la expresión *(arreglo+10). 

A pesar de que las funciones no son variables, es posible tener punteros a funciones. Estos punteros se pueden asignar, ubicar en arreglos, pasar como parámetros a otras funciones, o desreferenciar para ejecutar la función apuntada (sin necesidad de un operador). Los punteros a funciones se declaran siguiendo la estructura tipo_a_devolver (* nombre_función) (argumentos). A modo de ejemplo, sea la función:
	int suma(int a, int b) {
		return a + b;
	}
Podemos declarar un puntero a la misma con la sentencia:
	int (*puntero)(int, int) = suma;
y luego para ejecutarla basta con hacer: 
	puntero(1, 2);
Los punteros a funciones permiten manejar el flujo del programa de manera compleja, y son una herramienta muy valiosa cuando se utilizan correctamente. En este trabajo práctico se utilizan varias veces, independizando la función censar_arrecife de la función que muestra un Pokemon, o permitiendo trasladar Pokemones en base a un criterio definido por el usuario, indiferente al programador. 

Las funciones malloc y realloc permiten manipular memoria dinámica en la sección Heap del programa. Esta es una sección de memoria controlada exclusivamente por el programador. En particular, la función malloc recibe un número de bytes, intenta reservar esa cantidad de memoria, y devuelve un puntero al bloque reservado (o NULL en caso de error). 
Por otro lado, el objetivo de realloc es modificar el tamaño de un bloque de memoria existente. Recibe un puntero a ese bloque y el nuevo tamaño, y devuelve un puntero a la memoria modificada. No se garantiza que el puntero recibido y el nuevo sean iguales. 
Siempre que se reserva memoria en el Heap, esta debe ser liberada antes de finalizar la ejecución del programa mediante una llamada a free(ptr_a_memoria), de lo contrario, se dice que el programa "pierde memoria".
En el trabajo práctico se usan estas funciones para almacenar los datos del arrecife y del acuario. 



