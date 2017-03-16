#include "parserUtils.h"

t_queue* llamadas = NULL;
t_queue* retornos = NULL;

void parserUtilSetup(){
    llamadas = queue_create();
    retornos = queue_create();
}

Llamada* ultimaLlamada(){
    return queue_pop(llamadas);
}

Parametro* ultimoRetorno(){
    return queue_pop(retornos);
}


Llamada* crearLlamada(char* nombre, int cantidadParametros, ...){
    va_list parametrosVa;
    Parametro* parametros = malloc(10*sizeof(Parametro));

    va_start (parametrosVa , cantidadParametros);
    for (int x = 0; x < cantidadParametros; x++ ){
        parametros[x] = va_arg (parametrosVa, Parametro);
    }
    va_end ( parametrosVa );

    Llamada* retorno = malloc(sizeof(Llamada));
    retorno->nombre = nombre;
    retorno->parametros = parametros;
    return retorno;
}

Parametro* crearRetorno(){
    Parametro* ret = malloc(sizeof(Parametro));
    ret->puntero = 123;
    return ret;
}


t_puntero definirVariable(t_nombre_variable identificador_variable){
    queue_push(llamadas, crearLlamada("definirVariable", 1, identificador_variable));
    Parametro *retorno = crearRetorno();
    queue_push(retornos, retorno);
    return retorno->puntero;
};

t_puntero obtenerPosicionVariable(t_nombre_variable nombre_variable){
    queue_push(llamadas, crearLlamada("obtenerPosicionVariable", 1, nombre_variable));
    Parametro *retorno = crearRetorno();
    queue_push(retornos, retorno);
    return retorno->puntero;
}

t_valor_variable dereferenciar(t_puntero puntero){
    queue_push(llamadas, crearLlamada("dereferenciar", 1, puntero));
    Parametro *retorno = crearRetorno();
    queue_push(retornos, retorno);
    return retorno->valor_variable;
}

void asignar(t_puntero puntero, t_valor_variable valor_variable){
    queue_push(llamadas, crearLlamada("asignar", 2, puntero, valor_variable));
}

void irAlLabel(t_nombre_etiqueta nombre_etiqueta) {
    queue_push(llamadas, crearLlamada("irAlLabel", 1, nombre_etiqueta));
}

void assertDefinirVariable(t_nombre_variable valor){
    Llamada* llamada = ultimaLlamada();
    should_string(llamada->nombre) be equal to("definirVariable");
    should_char(llamada->parametros[0].nombre_variable) be equal to(valor);
    free(llamada->parametros);
    free(llamada);
}

void assertObtenerPosicion(t_nombre_variable nombre_variable){
    Llamada* llamada = ultimaLlamada();
    should_string(llamada->nombre) be equal to("obtenerPosicionVariable");
    should_char(llamada->parametros[0].nombre_variable) be equal to(nombre_variable);
    free(llamada->parametros);
    free(llamada);
}

void assertDereferenciar(t_puntero puntero){
    Llamada* llamada = ultimaLlamada();
    should_string(llamada->nombre) be equal to("dereferenciar");
    should_int(llamada->parametros[0].puntero) be equal to(puntero);
    free(llamada->parametros);
    free(llamada);
}

void assertAsignar(t_puntero puntero, t_valor_variable valor){
    Llamada* llamada = ultimaLlamada();
    should_string(llamada->nombre) be equal to("asignar");
    should_int(llamada->parametros[0].nombre_variable) be equal to(puntero);
    should_int(llamada->parametros[1].puntero) be equal to(valor);
    free(llamada->parametros);
    free(llamada);
}

void assertIrAlLabel(t_nombre_etiqueta nombre_etiqueta) {
    Llamada* llamada = ultimaLlamada();
    should_string(llamada->nombre) be equal to("irAlLabel");
    // TODO: review this :)
    should_string(llamada->parametros[0].nombre_etiqueta) be equal to(nombre_etiqueta);
    free(llamada->parametros);
    free(llamada);
}
