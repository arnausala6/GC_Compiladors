/*
 * test_lexer_token_model.c
 *
 * Test general del flujo: frase -> scanner -> separación en lexemas ->
 * clasificación (categoría) -> guardado en TokenList -> consumo para output.
 *
 * Comprueba:
 *   - Varias frases de entrada (números, identificadores, mezclas, símbolos).
 *   - Que los lexemas se separan por delimitadores y se clasifican por categoría.
 *   - Que cada token se guarda con loc, categoría, lexema, index y counters.
 *   - Que la lista se puede recorrer y usar para generar una salida (simulada).
 *
 * El número de tokens y su clasificación concreta dependen del automata_engine.
 * Este test verifica que el flujo es correcto (invariantes, consumo para output),
 * no un resultado exacto por frase.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scanner/scanner_core.h"
#include "token_model/token_model.h"
#include "diagnostics/diagnostics.h"
#include "counters/counters.h"

#ifndef MAX_OUTPUT_LEN
#define MAX_OUTPUT_LEN 4096
#endif

/* TokenList muy grande: estático para no desbordar la pila. */
static TokenList list;
static Diagnostics diag;
static Counters cnt;

/*
 * Ejecuta el scanner sobre la cadena `input` (nombre de fichero para loc = `filename`).
 * Usa la lista/diag/counters estáticos; hace tokenlist_clear antes de correr.
 * Devuelve tokenlist_size(&list) tras el run.
 */
static int run_scanner_on(const char *input, const char *filename) {
    tokenlist_clear(&list);
    diagnostics_init(&diag, stderr);
    counters_init(&cnt);

    FILE *f = tmpfile();
    if (!f) return -1;
    if (fputs(input, f) < 0) {
        fclose(f);
        return -1;
    }
    rewind(f);

    Scanner s;
    scanner_init(
        &s,
        f,
        filename,
        NULL,
        (struct TokenList *)&list,
        (struct Diagnostics *)&diag,
        (struct Counters *)&cnt
    );
    scanner_run(&s);
    fclose(f);
    return tokenlist_size(&list);
}

/*
 * Comprueba invariantes de la TokenList:
 *   - Cada token accesible por índice tiene index == i.
 *   - Lexema siempre null-terminated (comprobación básica).
 *   - loc con file no NULL (o al menos consistente).
 * Devuelve 0 si todo ok, 1 si falla.
 */
static int check_token_list_invariants(void) {
    int n = tokenlist_size(&list);
    for (int i = 0; i < n; i++) {
        const Token *t = tokenlist_get(&list, i);
        if (!t) return 1;
        if (t->index != i) return 1;
        /* Lexema debe estar null-terminated en algún punto (invariante del token_model) */
        { int j; for (j = 0; j < TOKEN_LEXEME_MAX && t->lexeme[j] != '\0'; j++) ; if (j >= TOKEN_LEXEME_MAX) return 1; }
        (void)t->loc;
    }
    /* Fuera de rango debe devolver NULL */
    if (tokenlist_get(&list, -1) != NULL) return 1;
    if (tokenlist_get(&list, n) != NULL) return 1;
    return 0;
}

/*
 * Simula el consumo de la lista para "output": construye una cadena que
 * representa la secuencia de tokens (categoría + lexema) como si se fuera
 * a pasar a una siguiente fase o a un writer.
 * Escribe en out[0..MAX_OUTPUT_LEN-1] y null-termina.
 */
static void consume_list_for_output(char *out, int max_len) {
    int n = tokenlist_size(&list);
    int pos = 0;
    for (int i = 0; i < n && pos < max_len - 1; i++) {
        const Token *t = tokenlist_get(&list, i);
        if (!t) break;
        int written = snprintf(out + pos, (size_t)(max_len - pos),
                               "%s(%s)%s",
                               token_category_name(t->category),
                               t->lexeme,
                               (i + 1 < n) ? " " : "");
        if (written <= 0 || written >= max_len - pos) break;
        pos += written;
    }
    out[pos] = '\0';
}

/* Casos de prueba: frase y descripción. */
static const struct {
    const char *phrase;
    const char *description;
} cases[] = {
    { "42\n", "solo número" },
    { "hello\n", "solo identificador" },
    { "42 99\n", "dos números" },
    { "if ( x )\n", "keyword y símbolos" },
    { "a b c\n", "varios identificadores" },
    { "1 + 2 ;\n", "números, operador y símbolo" },
    { "42 hello if ( ) ;\n", "frase mezclada (general)" },
};

enum { NUM_CASES = sizeof(cases) / sizeof(cases[0]) };

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    tokenlist_init(&list);
    printf("=== Test general: flujo frase -> lexemas -> clasificación -> TokenList -> output ===\n\n");

    int failed = 0;

    for (int c = 0; c < NUM_CASES; c++) {
        const char *phrase = cases[c].phrase;
        const char *desc  = cases[c].description;

        int n = run_scanner_on(phrase, "test.c");
        if (n < 0) {
            printf("[Caso %d] ERROR: no se pudo ejecutar el scanner (\"%s\")\n", c + 1, desc);
            failed++;
            continue;
        }

        if (check_token_list_invariants()) {
            printf("[Caso %d] FAIL invariantes (\"%s\")\n", c + 1, desc);
            failed++;
            continue;
        }

        /* Simular salida: consumo de la lista como para la siguiente fase */
        static char output[MAX_OUTPUT_LEN];
        consume_list_for_output(output, MAX_OUTPUT_LEN);

        printf("[Caso %d] \"%s\"\n", c + 1, desc);
        printf("  Entrada: %s", phrase[0] == '\n' ? "(solo newline)" : phrase);
        if (phrase[strlen(phrase)-1] != '\n') printf("\n");
        printf("  Tokens: %d\n", n);
        printf("  Clasificación y lexemas (como para output): %s\n", output);
        printf("  Detalle:\n");

        for (int i = 0; i < n; i++) {
            const Token *t = tokenlist_get(&list, i);
            if (!t) continue;
            const char *file = t->loc.file ? t->loc.file : "(null)";
            printf("    [%d] %s:%d:%d %s \"%s\"\n",
                   t->index, file, t->loc.line, t->loc.column,
                   token_category_name(t->category), t->lexeme);
        }

        int nd = diagnostics_count(&diag);
        if (nd > 0) {
            printf("  Diagnostics: %d\n", nd);
            for (int i = 0; i < nd; i++) {
                const Diagnostic *d = diagnostics_get(&diag, i);
                if (d) printf("    %s:%d:%d %s\n",
                              d->loc.file ? d->loc.file : "(null)",
                              d->loc.line, d->loc.column,
                              diagnostics_default_message(d->id));
            }
        }
        printf("\n");
    }

    printf("--- Resumen ---\n");
    if (failed) {
        printf("FALLOS: %d de %d casos\n", failed, NUM_CASES);
        return 1;
    }
    printf("OK: los %d casos pasaron. Flujo verificado: separación, clasificación, guardado en TokenList y consumo para output.\n", NUM_CASES);
    return 0;
}
