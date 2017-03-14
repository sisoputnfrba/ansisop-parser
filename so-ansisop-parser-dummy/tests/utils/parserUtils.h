#ifndef ANSISOP_PARSER_PARSERUTILS_H
#define ANSISOP_PARSER_PARSERUTILS_H

    #include <parser/parser.h>
    #include <cspecs/cspec.h>
    #include <commons/collections/queue.h>
    #include <stdarg.h>


    typedef union {
        t_nombre_variable nombre_variable;
        t_puntero puntero;
        t_valor_variable valor_variable;
    } Parametro;

    typedef struct {
        char* nombre;
        Parametro* parametros;
    } Llamada;

void parserUtilSetup();
Llamada* ultimaLlamada();
Parametro* ultimoRetorno();

t_puntero definirVariable(t_nombre_variable);
t_puntero obtenerPosicionVariable(t_nombre_variable);
t_valor_variable dereferenciar(t_puntero);
void asignar(t_puntero, t_valor_variable);


void assertDefinirVariable(t_nombre_variable);
void assertObtenerPosicion(t_nombre_variable);
void assertDereferenciar(t_puntero);
void assertAsignar(t_puntero, t_valor_variable);

#endif //ANSISOP_PARSER_PARSERUTILS_H
