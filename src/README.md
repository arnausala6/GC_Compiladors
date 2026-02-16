# P2 – Lexer (Scanner) – Código fuente

Este directorio contiene la implementación del **analizador léxico (scanner)** del compilador, correspondiente a la **Práctica 2 (P2)**.

---


## Especificación del lenguaje (resumen)

Según el handout:

| Elemento | Descripción |
|----------|-------------|
| **Operadores** | `=` `>` `+` `*` |
| **Caracteres especiales** | `(` `)` `;` `{` `}` `[` `]` `,` |
| **Números** | Solo enteros (dígitos 0-9, pueden empezar por 0). |
| **Keywords** | `if`, `else`, `while`, `return` (y tipos: `int`, `char`, `void` en el motor de autómatas). |
| **Identificadores** | Letras mayúsculas/minúsculas y dígitos, deben empezar por letra; no pueden ser keyword. |
| **Literales** | Texto entre `"` y `"`; el lexema **incluye** las comillas. Ej.: `"core dumped"`. |
| **Delimitadores** | Espacio, fin de línea, tabulador (no generan token). |
| **No reconocido** | Cualquier otro lexema genera error informativo y un token CAT_NONRECOGNIZED; el scanner continúa. |

El **alfabeto** del lenguaje se deriva de estos lexemas (ver documentación del desarrollador).

---

## Estructura de `src/`

```
src/
├── main.c              # Punto de entrada: argumentos, ficheros, scanner, salida
├── main.h              # AppConfig, FileCtx, flags (-i, -trace, -count, -mode)
├── utils_files.c       # Utilidades: nombres de fichero, timestamp, set_output_test_file
├── utils_files.h
├── CMakeLists.txt      # Declara utils y añade todos los submódulos
│
├── module_args/        # Argumentos de línea de comandos
│   ├── module_args.c   # process_arguments, print_help
│   ├── module_args.h
│   └── CMakeLists.txt
│
├── diagnostics/        # Gestión de errores (P2 y futuro parser)
│   ├── diagnostics.c   # diagnostics_init, add_error_at, mensajes por ErrorId
│   ├── diagnostics.h   # Diagnostics, ErrorId, CompilerPhase (PHASE_SCANNER)
│   └── CMakeLists.txt
│
├── counters/           # Conteo de operaciones (COUNTCOMP, COUNTIO, COUNTGEN)
│   ├── counters.c      # counters_init, add_comp/io/gen, counters_report
│   ├── counters.h      # Counters, CountConfig (COUNTOUT, COUNTFILE)
│   └── CMakeLists.txt
│
├── automata_engine/    # Motor de DFAs para reconocer categorías de token
│   ├── automata_engine.c  # 6 DFAs: number, identifier, keyword, literal, operator, specialchar
│   ├── automata_engine.h # DFA, TokenCategory, automata_engine_step, reset
│   └── CMakeLists.txt
│
├── token_model/        # Modelo de token y lista de tokens
│   ├── token_model.c   # tokenlist_init/add/get/size/clear, token_category_name
│   ├── token_model.h   # Token, TokenList, SrcLoc (vía scanner_core.h)
│   └── CMakeLists.txt
│
├── scanner/            # Núcleo del lexer
│   ├── scanner_core.c  # scanner_init, scanner_run, read_char, scan_next_token, emit_token
│   ├── scanner_core.h  # Scanner, SrcLoc, scanner_init, scanner_run
│   └── CMakeLists.txt
│
└── output_writer/      # Escritura de tokens al fichero de salida
    ├── output_writer.c # output_writer_init, write_token, close (formato release/debug)
    ├── output_writer.h # OutputWriter, OutFormat
    └── CMakeLists.txt
```

---

## Descripción de cada apartado

### `main.c` / `main.h`

- **main.c:** Orquesta el flujo: log (`ofile`), `process_arguments`, apertura de ficheros (entrada, `.cscn`, opcional `.dbgcnt`), `diagnostics_init`, `counters_init`, `tokenlist_init`, `output_writer_init`, `scanner_init`, `scanner_run`, escritura de cada token con `output_writer_write_token`, informe de contadores al `.dbgcnt` si `-count`, cierre de ficheros.
- **main.h:** Define `AppConfig` (outformat, debug_on, count_enabled, countout), `FileCtx` (input_file, output_file, dbgcnt_file), constantes de flags (`FLAG_INPUT`, `FLAG_TRACE`, `FLAG_COUNT`, `FLAG_MODE`) y `extern FILE *ofile` para el log.

### `utils_files.c` / `utils_files.h`

- Generación de nombres de fichero con timestamp y `set_output_test_file`: abre el log de ejecución (p. ej. `./proj_modules_template.log`) o usa stdout. Usado por `main` y por `module_args` (si se usa `ofile`).

### `module_args/`

- Parsea la línea de comandos: `-i <fichero>` (obligatorio), `-mode release|debug`, `-trace`, `-count`. Rellena `AppConfig` y el nombre del fichero de entrada. Sin `-i` devuelve error e imprime la ayuda.

### `diagnostics/`

- **Error handling (handout §5):** Errores identificados por **ErrorId** (p. ej. ERR_NONRECOGNIZED, ERR_LEXEME_TOO_LONG) y **CompilerPhase** (PHASE_SCANNER; luego parser). Mismo mensaje para el mismo error; se pueden añadir detalles (ubicación, datos). Los mensajes se escriben en el stream elegido según la macro **DEBUG** (configurado en `diagnostics_init`).

### `counters/`

- **Conteo de operaciones (handout §7):** COUNTCOMP (comparaciones), COUNTIO (E/S por caracteres leídos/escritos), COUNTGEN (resto de instrucciones). Solo activo con **COUNTCONFIG** (en build Debug). **COUNTOUT** / **COUNTFILE** indican dónde se envían los mensajes de conteo (fichero de tokens o fichero `.dbgcnt`). `counters_report` escribe línea, función, incrementos y totales.

### `automata_engine/`

- Mantiene 6 DFAs en paralelo (número, identificador, keyword, literal, operador, carácter especial). `automata_engine_reset` y `automata_engine_step(ch, ...)` actualizan estados; se usa el mejor “accepting” para decidir la categoría del token. Los keywords tienen prioridad sobre identificador cuando coinciden.

### `token_model/`

- Define `Token` (lexema, categoría, SrcLoc, index, counters_at_emit) y `TokenList`. El scanner llama a `tokenlist_add` por cada token reconocido. Compatible con `output_writer` y con el futuro parser.

### `scanner/`

- Lee el fichero de entrada carácter a carácter, usa el motor de autómatas para reconocer lexemas, rellena la `TokenList` y reporta errores vía `diagnostics_add_error_at`. Soporta lookahead de un carácter (pending char) y límite de longitud de lexema (ERR_LEXEME_TOO_LONG).

### `output_writer/`

- Escribe cada token en el fichero de salida `.cscn` en formato release (compacto) o debug (con ubicación y lexema), según `OutFormat` y `debug_on`.

---

## Cómo compilar

Desde la **raíz del proyecto**:

```bash
mkdir build
cd build
cmake .. 
cmake --build . --config Debug

cd Debug

.\scanner_main.exe -i [..\..\tests\(nombre del test).c] [mode] [count] 

```

**Opciones (según handout y `main.h`):**

| Opción | Descripción |
|--------|-------------|
| `-i <fichero>` | **(Obligatorio)** Fichero de entrada a analizar. |
| `-mode release` | Formato de salida release (por defecto). |
| `-mode debug` | Formato de salida debug (más detalle por token). |
| `-trace` | Activa modo traza (DEBUG=1 a nivel de uso; puede afectar dónde se escriben los diagnósticos). |
| `-count` | Activa conteo de operaciones y escribe el informe en un fichero `.dbgcnt`. |

Ejemplo con todas las opciones:

```bash
./scanner_main -i programa.c -mode debug -trace -count
```

---

## Ficheros de salida

| Fichero | Descripción |
|---------|-------------|
| **`<nombre>.cscn`** | Salida del scanner: un token por línea (o formato compacto en release), con `[línea:columna]` categoría y lexema. |
| **`<nombre>.dbgcnt`** | Solo si se usa `-count`: informe de conteo (COUNTCOMP, COUNTIO, COUNTGEN) por función y totales. Solo tiene datos si se compiló en Debug con COUNTCONFIG. |
| **Log (p. ej. `./proj_modules_template.log`)** | Traza de ejecución del programa (qué ficheros se abren, etc.). No es la salida de errores del compilador (eso lo gestiona **diagnostics**). |

Los **mensajes de error** del scanner (no reconocido, lexema demasiado largo, etc.) se escriben según la configuración de **diagnostics** (según DEBUG: stdout o fichero de salida).

---
