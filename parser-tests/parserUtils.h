#ifndef ANSISOP_PARSER_PARSERUTILS_H
#define ANSISOP_PARSER_PARSERUTILS_H

    #include <parser/parser.h>
    #include <cspecs/cspec.h>
    #include <commons/collections/queue.h>
    #include <commons/log.h>
    #include <stdarg.h>

    #include <time.h>
    #include <stdlib.h>


    typedef union {
        t_nombre_variable nombre_variable;
        t_puntero puntero;
        t_valor_variable valor_variable;
        t_nombre_etiqueta nombre_etiqueta;
        char* string;
        t_descriptor_archivo descriptor_archivo;
        t_direccion_archivo direccion_archivo;
        t_banderas banderas;
    } Parametro;

    typedef struct {
        char* nombre;
        Parametro* parametros;
    } Llamada;

void parserUtilSetup(char *archivo, bool imprimirEnPantalla);
void parserUtilTearDown();

void limpiarElContextoDeEjecucion();
Llamada* ultimaLlamada();
Parametro* ultimoRetorno();

t_puntero definirVariable(t_nombre_variable);
t_puntero obtenerPosicionVariable(t_nombre_variable);
t_valor_variable dereferenciar(t_puntero);
void asignar(t_puntero, t_valor_variable);
void irAlLabel(t_nombre_etiqueta nombre_etiqueta);
t_puntero alocar(t_valor_variable);
void liberar(t_puntero);
t_descriptor_archivo abrir(t_direccion_archivo, t_banderas);
void borrar(t_descriptor_archivo);
void cerrar(t_descriptor_archivo);
void moverCursor(t_descriptor_archivo, t_valor_variable);
void escribir(t_descriptor_archivo, void *, t_valor_variable);
void leer(t_descriptor_archivo, t_puntero, t_valor_variable);

t_puntero assertDefinirVariable(t_nombre_variable);
t_puntero assertObtenerPosicion(t_nombre_variable);
t_valor_variable assertDereferenciar(t_puntero);
void assertAsignar(t_puntero, t_valor_variable);
void assertIrAlLabel(t_nombre_etiqueta nombre_etiqueta);
t_puntero assertMalloc(t_valor_variable);
void assertLiberar(t_puntero);
void assertEscribir(t_descriptor_archivo, void*, t_valor_variable);
t_descriptor_archivo assertAbrir(t_direccion_archivo, t_banderas);
void assertBorrar(t_descriptor_archivo);
void assertCerrar(t_descriptor_archivo);
void assertMoverCursor(t_descriptor_archivo, t_valor_variable);
void assertLeer(t_descriptor_archivo, t_puntero, t_valor_variable);

#endif //ANSISOP_PARSER_PARSERUTILS_H
