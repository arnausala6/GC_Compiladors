Este proyecto implementa un parser bottom-up basado en un
autómata Shift/Reduce (SRA). El parser analiza una secuencia
de tokens generada por el scanner y determina si la entrada
pertenece al lenguaje definido por la gramática.

El sistema combina dos estructuras principales:

• Un autómata DFA basado en tablas ACTION y GOTO
• Una pila (stack) que almacena pares (state, symbol)

Las operaciones del parser son:

SHIFT → consume el token y avanza el autómata
REDUCE → aplica una producción de la gramática
ACCEPT → la cadena pertenece al lenguaje
ERROR → no existe transición válida

GRAMÁTICA

s → e
e → e + t | t
t → t * f | f
f → ( e ) | NUM

Terminales: + * ( ) NUM $

COMPILACIÓN

Desde la raíz del proyecto:

mkdir build
cd build
cmake ..
cmake --build .

EJECUCIÓN

Ejemplo con el archivo de prueba:

.\Debug\parser_main.exe ..\parser\test_expr.cscn

OUTPUT

El parser genera un archivo de debug con el nombre:

<input>_p3dbg.txt

Este archivo contiene la traza completa del parser
mostrando las operaciones SHIFT, REDUCE y el estado
de la pila en cada paso.

AUTORES

Raúl Díaz
Yeray Espinosa
Wassim Ibn Tahar
Iván Jiménez
Arnau Sala
Iván Torres
