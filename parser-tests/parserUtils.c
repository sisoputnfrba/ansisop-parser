#include <parser/parser.h>
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
        parametros[x] = va_arg (parametrosVa, Parametro);
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

#define CON_RETORNO_PUNTERO     CON_RETORNO("p", puntero)
#define CON_RETORNO_VALOR     CON_RETORNO("d", valor_variable)
#define CON_RETORNO_DESCRIPTOR CON_RETORNO_VALOR

#define CON_RETORNO(FORMATO, TIPO) \
Parametro *retorno = crearRetorno(); \
queue_push(retornos, retorno);\
log_trace(logger, "\tdevuelve [%" FORMATO "]", retorno->TIPO);\
return retorno->puntero


t_puntero definirVariable(t_nombre_variable identificador_variable){
    log_trace(logger, "Definir Variable [%c]", identificador_variable);
    queue_push(llamadas, crearLlamada("definirVariable", 1, identificador_variable));
    CON_RETORNO_PUNTERO;
};

t_puntero obtenerPosicionVariable(t_nombre_variable nombre_variable){
    log_trace(logger, "Obtener posicion variable [%c]", nombre_variable);

    queue_push(llamadas, crearLlamada("obtenerPosicionVariable", 1, nombre_variable));
    CON_RETORNO_PUNTERO;
}

t_valor_variable dereferenciar(t_puntero puntero){
    log_trace(logger, "Dereferenciar [%p]", puntero);

    queue_push(llamadas, crearLlamada("dereferenciar", 1, puntero));
    CON_RETORNO_VALOR;
}

void asignar(t_puntero puntero, t_valor_variable valor_variable){
    log_trace(logger, "Asignar a [%p] el valor [%d]", puntero, valor_variable);
    queue_push(llamadas, crearLlamada("asignar", 2, puntero, valor_variable));
}

void irAlLabel(t_nombre_etiqueta nombre_etiqueta) {
    log_trace(logger, "Ir al Label [%s]", nombre_etiqueta);
    queue_push(llamadas, crearLlamada("irAlLabel", 1, string_duplicate(nombre_etiqueta)));
}

t_puntero alocar(t_valor_variable espacio){
    log_trace(logger, "Reserva [%d] espacio", espacio);

    queue_push(llamadas, crearLlamada("alocar", 1, espacio));
    CON_RETORNO_PUNTERO;
}

void liberar(t_puntero puntero){
    log_trace(logger, "Reserva [%p] espacio", puntero);

    queue_push(llamadas, crearLlamada("liberar", 1, puntero));
}

char* boolToChar(bool boolean) {
    return boolean ? "✔" : "✖";
}

t_descriptor_archivo abrir(t_direccion_archivo direccion, t_banderas banderas){
    log_trace(logger, "Abrir [%s] Lectura: %s. Escritura: %s, Creacion: %s", direccion,
              boolToChar(banderas.lectura), boolToChar(banderas.escritura), boolToChar(banderas.creacion));

    queue_push(llamadas, crearLlamada("abrir", 2, direccion, banderas));
    CON_RETORNO_DESCRIPTOR;
}

void borrar(t_descriptor_archivo descriptor){
    log_trace(logger, "Borrar [%d]", descriptor);
    queue_push(llamadas, crearLlamada("borrar", 1, descriptor));
}

void cerrar(t_descriptor_archivo descriptor){
    log_trace(logger, "Cerrar [%d]", descriptor);
    queue_push(llamadas, crearLlamada("cerrar", 1, descriptor));
}

void moverCursor(t_descriptor_archivo descriptor, t_valor_variable posicion){
    log_trace(logger, "Mover descriptor [%d] a [%d]", descriptor, posicion);
    queue_push(llamadas, crearLlamada("mover", 2, descriptor, posicion));

}

void escribir(t_descriptor_archivo desc, void * informacion, t_valor_variable tamanio){
    log_trace(logger, "Escribir [%.*s]:%d a [%d]", tamanio, informacion, tamanio, desc);

    queue_push(llamadas, crearLlamada("escribir", 3, desc, string_duplicate(informacion), tamanio));
}

void leer(t_descriptor_archivo descriptor, t_puntero informacion, t_valor_variable tamanio){
    log_trace(logger, "Leer desde [%d] a [%p] con tamaño [%d]", descriptor, informacion, tamanio);

    queue_push(llamadas, crearLlamada("leer", 3, descriptor, informacion, tamanio));
}

t_puntero assertDefinirVariable(t_nombre_variable valor){
    Llamada* llamada = ultimaLlamada();
    should_string(llamada->nombre) be equal to("definirVariable");
    should_char(llamada->parametros[0].nombre_variable) be equal to(valor);
    free(llamada->parametros);
    free(llamada);
    return ultimoRetorno()->puntero;
}

t_puntero assertObtenerPosicion(t_nombre_variable nombre_variable){
    Llamada* llamada = ultimaLlamada();
    should_string(llamada->nombre) be equal to("obtenerPosicionVariable");
    should_char(llamada->parametros[0].nombre_variable) be equal to(nombre_variable);
    free(llamada->parametros);
    free(llamada);
    return ultimoRetorno()->puntero;
}

t_valor_variable assertDereferenciar(t_puntero puntero){
    Llamada* llamada = ultimaLlamada();
    should_string(llamada->nombre) be equal to("dereferenciar");
    should_int(llamada->parametros[0].puntero) be equal to(puntero);
    free(llamada->parametros);
    free(llamada);
    return ultimoRetorno()->valor_variable;
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
    should_string(llamada->parametros[0].nombre_etiqueta) be equal to(nombre_etiqueta);
    free(llamada->parametros);
    free(llamada);
}

t_puntero assertMalloc(t_valor_variable espacio){
    Llamada* llamada = ultimaLlamada();
    should_string(llamada->nombre) be equal to("alocar");
    should_int(llamada->parametros[0].valor_variable) be equal to(espacio);
    free(llamada->parametros);
    free(llamada);
    return ultimoRetorno()->puntero;
}

void assertLiberar(t_puntero puntero){
    Llamada* llamada = ultimaLlamada();
    should_string(llamada->nombre) be equal to("liberar");
    should_int(llamada->parametros[0].puntero) be equal to(puntero);
    free(llamada->parametros);
    free(llamada);
}

void assertEscribir(t_descriptor_archivo descriptor, void * informacion, t_valor_variable tamanio){
    Llamada* llamada = ultimaLlamada();
    should_string(llamada->nombre) be equal to("escribir");
    should_int(llamada->parametros[0].descriptor_archivo) be equal to(descriptor);
    should_string(llamada->parametros[1].string) be equal to(informacion);
    should_int(llamada->parametros[2].valor_variable) be equal to(tamanio);
    free(llamada->parametros[1].string);
    free(llamada->parametros);
    free(llamada);
}

t_descriptor_archivo assertAbrir(t_direccion_archivo direccion, t_banderas banderas){
    Llamada* llamada = ultimaLlamada();
    should_string(llamada->nombre) be equal to("abrir");
    should_string(llamada->parametros[0].direccion_archivo) be equal to(direccion);
    should_int(llamada->parametros[1].banderas.lectura) be equal to(banderas.lectura);
    should_int(llamada->parametros[1].banderas.escritura) be equal to(banderas.escritura);
    should_int(llamada->parametros[1].banderas.creacion) be equal to(banderas.creacion);
    free(llamada->parametros[0].direccion_archivo);
    free(llamada->parametros);
    free(llamada);
    return ultimoRetorno()->descriptor_archivo;
}

void assertBorrar(t_descriptor_archivo descriptor){
    Llamada* llamada = ultimaLlamada();
    should_string(llamada->nombre) be equal to("borrar");
    should_int(llamada->parametros[0].descriptor_archivo) be equal to(descriptor);
    free(llamada->parametros);
    free(llamada);
}

void assertCerrar(t_descriptor_archivo descriptor){
    Llamada* llamada = ultimaLlamada();
    should_string(llamada->nombre) be equal to("cerrar");
    should_int(llamada->parametros[0].descriptor_archivo) be equal to(descriptor);
    free(llamada->parametros);
    free(llamada);
}

void assertMoverCursor(t_descriptor_archivo descriptor, t_valor_variable posicion){
    Llamada* llamada = ultimaLlamada();
    should_string(llamada->nombre) be equal to("mover");
    should_int(llamada->parametros[0].descriptor_archivo) be equal to(descriptor);
    should_int(llamada->parametros[1].valor_variable) be equal to(posicion);
    free(llamada->parametros);
    free(llamada);
}

void assertLeer(t_descriptor_archivo descriptor, t_puntero informacion, t_valor_variable tamanio){
    Llamada* llamada = ultimaLlamada();
    should_string(llamada->nombre) be equal to("leer");
    should_int(llamada->parametros[0].descriptor_archivo) be equal to(descriptor);
    should_int(llamada->parametros[1].puntero) be equal to(informacion);
    should_int(llamada->parametros[2].valor_variable) be equal to(tamanio);
    free(llamada->parametros);
    free(llamada);
}