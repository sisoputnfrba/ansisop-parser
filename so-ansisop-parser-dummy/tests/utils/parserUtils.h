#ifndef ANSISOP_PARSER_PARSERUTILS_H
#define ANSISOP_PARSER_PARSERUTILS_H

    #include <parser/parser.h>
    #include <cspecs/cspec.h>
    #include <commons/collections/queue.h>
    #include <stdarg.h>


    typedef union {
        t_nombre_variable nombre_variable;
        t_puntero puntero;
    } Parametro;

    typedef struct {
        char* nombre;
        Parametro* parametros;
    } Llamada;

void parserUtilSetup();
Llamada* ultimaLlamada();
Parametro* ultimoRetorno();

t_puntero definirVariable(t_nombre_variable identificador_variable);

#endif //ANSISOP_PARSER_PARSERUTILS_H
