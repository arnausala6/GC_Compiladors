# P3 – Parser (Shift/Reduce) – Código fuente

Este directorio contiene la implementación del **analizador sintáctico (parser)** del compilador, correspondiente a la **Práctica 3 (P3)**.

El parser recibe como entrada el **TokenStream generado por el scanner (P2)** y determina si la secuencia de tokens pertenece al lenguaje definido por la gramática.

El sistema implementa un **Shift-Reduce Automaton (SRA)** basado en las tablas **ACTION** y **GOTO**, utilizando una **pila (stack)** para gestionar los estados del autómata.

---

## Especificación del lenguaje (resumen)

Según el handout del proyecto:

| Elemento | Descripción |
|----------|-------------|
| **Terminales** | `+ * ( ) NUM $` |
| **No terminales** | `s, e, t, f` |
| **Símbolo inicial** | `s` |
| **Entrada del parser** | Secuencia de tokens `.cscn` generada por el scanner |
| **Símbolo fin de entrada** | `$` |

---

## Gramática utilizada

```
s → e
e → e + t
e → t
t → t * f
t → f
f → ( e )
f → NUM
```

Esta gramática permite reconocer **expresiones aritméticas con precedencia y paréntesis**.

---

## Operaciones del parser

El autómata del parser puede ejecutar cuatro acciones:

| Acción | Descripción |
|------|-------------|
| **SHIFT** | Consume el token actual y avanza al siguiente estado del autómata |
| **REDUCE** | Aplica una producción de la gramática |
| **ACCEPT** | La cadena pertenece al lenguaje |
| **ERROR** | No existe transición válida en la tabla ACTION |

La transición del parser se decide mediante:

```
ACTION[state][symbol]
```

donde:

```
state  = estado actual del autómata
symbol = símbolo del token lookahead
```

---

## Estructura de `parser/`

```
parser/
├── main.c                # Punto de entrada del parser
├── utils_files.c         # Utilidades de ficheros
├── CMakeLists.txt
│
├── language/             # Carga de grammar y tablas ACTION/GOTO
│   ├── language.c
│   ├── language.h
│   └── CMakeLists.txt
│
├── modulo_automata/      # Autómata del parser
│   ├── automata.c
│   ├── automata.h
│   └── CMakeLists.txt
│
├── module_stack/         # Implementación de la pila
│   ├── stack.c
│   ├── stack.h
│   └── CMakeLists.txt
│
├── modulo_tokenlist/     # Lectura del TokenStream (.cscn)
│   ├── tokenlist.c
│   ├── tokenlist.h
│   └── CMakeLists.txt
│
├── module_debug/         # Escritura del output de debug
│   ├── debug.c
│   ├── debug.h
│   └── CMakeLists.txt
│
├── module_args/          # Argumentos de entrada
│   ├── module_args.c
│   ├── module_args.h
│   └── CMakeLists.txt
│
└── sra_engine/           # Motor principal del parser
    ├── sra_engine.c
    ├── sra_engine.h
    └── CMakeLists.txt
```

---

## Descripción de cada módulo

### main.c

- Punto de entrada del parser.
- Carga el archivo `.cscn` generado por el scanner.
- Inicializa las estructuras principales:
  - **Language**
  - **TokenStream**
  - **Stack**
  - **SRA Engine**
- Ejecuta el parser y genera el **output de debug**.

### language/

Carga el archivo de especificación del lenguaje y almacena:

- **Producciones**
- **Tabla ACTION**
- **Tabla GOTO**
- **Terminales y no terminales**

Estas tablas son utilizadas por el **autómata del parser** para decidir las transiciones.

### modulo_automata/

Gestiona el **estado del autómata del parser**.

La acción se obtiene consultando:

```
ACTION[state][symbol]
```

y determina si se debe realizar **SHIFT, REDUCE, ACCEPT o ERROR**.

### module_stack/

Implementa la **pila del parser**.

Cada elemento contiene:

```
StackItem
 - state
 - symbol
```

Ejemplo de pila durante la ejecución:

```
[st:5, sym:NUM]
[st:3, sym:+]
[st:0, sym:$]
```

### modulo_tokenlist/

Carga el **TokenStream** generado por el scanner (`.cscn`) y permite acceder al **lookahead token**.

### sra_engine/

Contiene el **motor principal del parser**.

Algoritmo simplificado:

```
while (true)

    action = ACTION[state][lookahead]

    if SHIFT
        push(token)
        push(new_state)
        consume token

    if REDUCE
        pop |β|
        state = stack.top
        goto = GOTO[state][A]
        push(A)
        push(goto)

    if ACCEPT
        finish

    if ERROR
        stop
```

---

## Cómo compilar

Desde la **raíz del proyecto**:

```
mkdir build
cd build
cmake ..
cmake --build .
```

El ejecutable generado será:

```
build/Debug/parser_main.exe   (Windows)
build/parser_main             (Linux / WSL)
```

---

## Ejecución

Ejemplo usando el archivo de prueba:

```
.\Debug\parser_main.exe ..\parser\test_expr.cscn
```

El archivo `.cscn` debe haber sido generado previamente por el **scanner (P2)**.

---

## Ficheros de salida

| Fichero | Descripción |
|--------|-------------|
| **<nombre>_p3dbg.txt** | Traza completa del parser |
| **Log de ejecución** | Información interna del programa |

Ejemplo de salida del parser:

```
[0] | INIT      | - -> 0 | NUM | STACK: [st:0, sym:$]
[1] | SHIFT 5   | 0 -> 5 | +   | STACK: [st:0,$] [st:5,NUM]
[1] | REDUCE 6  | 5 -> 3 | +   | STACK: [st:0,$] [st:3,F]
[3] | ACCEPT    | 1 -> - | $   | STACK: [st:0,$] [st:1,E]
```

---

## Autores

**Raúl Díaz**  
**Yeray Espinosa**  
**Wassim Ibn Tahar**  
**Iván Jiménez**  
**Arnau Sala**  
**Iván Torres**
