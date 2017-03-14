#include "parserUtils.h"

t_queue* llamadas = NULL;
t_queue* retorno = NULL;

void parserUtilSetup(){
    llamadas = queue_create();
    retorno = queue_create();
}

Llamada* ultimaLlamada(){
    return queue_pop(llamadas);
}

Parametro* ultimoRetorno(){
    return queue_pop(retorno);
}


Llamada* crearLlamada(char* nombre, int cantidadParametros, ...){
    va_list parametrosVa;
    Parametro* parametros = malloc(10*sizeof(Parametro));

    va_start (parametrosVa , cantidadParametros);
    for (int x = 0; x < cantidadParametros; x++ ){
        parametros[x] = va_arg ( parametrosVa, Parametro);
    }
    va_end ( parametrosVa );

    Llamada* retorno = malloc(sizeof(Llamada));
    retorno->nombre = nombre;
    retorno->parametros = parametros;
    return retorno;
}

Parametro* crearRetorno(){
    Parametro* ret = malloc(sizeof(Parametro));
    ret->puntero = 552;
    return ret;
}


t_puntero definirVariable(t_nombre_variable identificador_variable){
    queue_push(llamadas, crearLlamada("definirVariable", 1, identificador_variable));
    queue_push(retorno, crearRetorno());
};
