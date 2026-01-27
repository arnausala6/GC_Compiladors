    /*
    
    archivo: prueba_preprocesador.c
    descripcion: archivo con defines e ifdef para pruebas*/

    /* ================================
    DEFINES
    ================================ */

    #define MAX_COUNT 100
    #define MODE_DEBUG 1

    // Comentario simple antes del codigo

    /* ================================
    BLOQUES CON IFDEF
    ================================ */

    int valor = 0;

    #ifdef ENABLE_LOGS
    // Codigo que solo existe si ENABLE_LOGS esta definido
    int logs_activados = 1;
    #endif

    #ifdef MODE_DEBUG
    // Seccion de depuracion
    int debug_level = MODE_DEBUG;
    #endif

    /* ================================
    MAS CODIGO
    ================================ */

    void funcion_ejemplo(void)
    {
    #ifdef ENABLE_LOGS
        // Incremento si logs estan activos
        valor = valor + 1;
    #endif

    #ifdef MODE_DEBUG
        // Incremento extra en modo debug
        valor = valor + 10;
        #ifdef ENABLE_LOGS
            // Incremento extra en modo debug si logs estan activos
            valor = valor + 100;
            #ifdef MODE_DEBUG
                // Incremento extra en modo debug si logs estan activos y modo debug
                valor = valor + 1000;
            #endif
        #endif
        #ifdef MODE_DEBUG
            // Incremento extra en modo debug
            valor = valor + 20;
            #include "..\..\tests\t4.h"
        #endif
    #endif

        // Incremento normal
        valor = valor + 2;
    }

    /* Fin del archivo */