#include "parserUtils.h"

t_queue* llamadas = NULL;
t_queue* retornos = NULL;
t_log* logger = NULL;

void parserUtilSetup(char *archivo, bool imprimirEnPantalla) {
    llamadas = queue_create();
    retornos = queue_create();
    srand((unsigned int) time(NULL));

    logger = log_create(archivo, "TEST", imprimirEnPantalla, LOG_LEVEL_TRACE);
}

void parserUtilTearDown(){
    limpiarElContextoDeEjecucion();
    queue_destroy(llamadas);
    queue_destroy(retornos);
    log_destroy(logger);
}


void limpiarElContextoDeEjecucion(){
    queue_clean_and_destroy_elements(llamadas, free);
    queue_clean_and_destroy_elements(retornos, free);
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
        parametros[x] = va_arg ( parametrosVa, Parametro);
    }
    va_end ( parametrosVa );

    Llamada* retorno = malloc(sizeof(Llamada));
    retorno->nombre = nombre;
    retorno->parametros = parametros;
    return retorno;
}

Parametro* crearRetorno(){
    static int num = 4;
    Parametro* ret = malloc(sizeof(Parametro));
    ret->valor_variable = num++;
    return ret;
}


t_puntero definirVariable(t_nombre_variable identificador_variable){
    log_trace(logger, "Definir variable [%c]", identificador_variable);

    queue_push(llamadas, crearLlamada("definirVariable", 1, identificador_variable));
    Parametro *retorno = crearRetorno();
    queue_push(retornos, retorno);

    log_trace(logger, "\tdevuelve [%p]", retorno->puntero);
    return retorno->puntero;
};

t_puntero obtenerPosicionVariable(t_nombre_variable nombre_variable){
    log_trace(logger, "Obtener posicion variable [%c]", nombre_variable);

    queue_push(llamadas, crearLlamada("obtenerPosicionVariable", 1, nombre_variable));
    Parametro *retorno = crearRetorno();
    queue_push(retornos, retorno);

    log_trace(logger, "\tdevuelve [%p]", retorno->puntero);
    return retorno->puntero;
}

t_valor_variable dereferenciar(t_puntero puntero){
    log_trace(logger, "Dereferenciar [%p]", puntero);

    queue_push(llamadas, crearLlamada("dereferenciar", 1, puntero));
    Parametro *retorno = crearRetorno();
    queue_push(retornos, retorno);

    log_trace(logger, "\tdevuelve [%d]", retorno->valor_variable);
    return retorno->valor_variable;
}

void asignar(t_puntero puntero, t_valor_variable valor_variable){
    log_trace(logger, "Asignar a [%p] el valor [%d]", puntero, valor_variable);
    queue_push(llamadas, crearLlamada("asignar", 2, puntero, valor_variable));
}


t_puntero alocar(t_valor_variable espacio){
    log_trace(logger, "Reserva [%d] espacio", espacio);

    queue_push(llamadas, crearLlamada("alocar", 1, espacio));
    Parametro *retorno = crearRetorno();
    queue_push(retornos, retorno);

    log_trace(logger, "\tdevuelve [%p]", retorno->puntero);
    return retorno->puntero;
}


t_puntero assertDefinirVariable(t_nombre_variable valor){
    Llamada* llamada = ultimaLlamada();
    should_string(llamada->nombre) be equal to("definirVariable");
    should_char(llamada->parametros[0].nombre_variable) be equal to(valor);
    return ultimoRetorno()->puntero;
}

t_puntero assertObtenerPosicion(t_nombre_variable nombre_variable){
    Llamada* llamada = ultimaLlamada();
    should_string(llamada->nombre) be equal to("obtenerPosicionVariable");
    should_char(llamada->parametros[0].nombre_variable) be equal to(nombre_variable);
    return ultimoRetorno()->puntero;
}

t_valor_variable assertDereferenciar(t_puntero puntero){
    Llamada* llamada = ultimaLlamada();
    should_string(llamada->nombre) be equal to("dereferenciar");
    should_int(llamada->parametros[0].puntero) be equal to(puntero);
    return ultimoRetorno()->valor_variable;
}

void assertAsignar(t_puntero puntero, t_valor_variable valor){
    Llamada* llamada = ultimaLlamada();
    should_string(llamada->nombre) be equal to("asignar");
    should_int(llamada->parametros[0].nombre_variable) be equal to(puntero);
    should_int(llamada->parametros[1].puntero) be equal to(valor);
}

t_puntero assertMalloc(t_valor_variable espacio){
    Llamada* llamada = ultimaLlamada();
    should_string(llamada->nombre) be equal to("alocar");
    should_int(llamada->parametros[0].valor_variable) be equal to(espacio);
    return ultimoRetorno()->puntero;
}