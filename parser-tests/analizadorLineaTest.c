#include <parser/parser.h>
#include <cspecs/cspec.h>
#include "parserUtils.h"

AnSISOP_funciones *funciones = NULL;
AnSISOP_kernel *kernel = NULL;
bool imprimirEnPantalla = true;

context (parser) {

    void setup(){
        funciones = malloc(sizeof(AnSISOP_funciones));
        kernel = malloc(sizeof(AnSISOP_kernel));
        parserUtilSetup(tmpnam(NULL), imprimirEnPantalla);

        funciones->AnSISOP_definirVariable = definirVariable;
        funciones->AnSISOP_obtenerPosicionVariable = obtenerPosicionVariable;
        funciones->AnSISOP_obtenerValorCompartida = obtenerValorCompartida;
        funciones->AnSISOP_dereferenciar = dereferenciar;
        funciones->AnSISOP_asignar = asignar;
        funciones->AnSISOP_irAlLabel = irAlLabel;


        kernel->AnSISOP_reservar = alocar;
        kernel->AnSISOP_liberar = liberar;

        kernel->AnSISOP_abrir = abrir;
        kernel->AnSISOP_borrar = borrar;
        kernel->AnSISOP_cerrar = cerrar;
        kernel->AnSISOP_moverCursor = moverCursor;
        kernel->AnSISOP_escribir = escribir;
        kernel->AnSISOP_leer = leer;
    };

    setup();

    describe("Si al parser") {
        before {
            limpiarElContextoDeEjecucion();
        } end

        it("definicion de variables") {
            analizadorLinea("variables x, a, g", funciones, kernel);
                assertDefinirVariable('x');
                assertDefinirVariable('a');
                assertDefinirVariable('g');
        } end

         it("asignacion de variables") {
            analizadorLinea("x = a+3", funciones, kernel);
                t_puntero posicionA = assertObtenerPosicion('a');
                t_valor_variable valorA = assertDereferenciar(posicionA);
                t_puntero posicionX = assertObtenerPosicion('x');
                assertAsignar(posicionX, valorA+3);
        } end

        it("asignacion de punteros de variables") {
           analizadorLinea("*x=3", funciones, kernel);
           t_valor_variable valorX = assertDereferenciar(assertObtenerPosicion('x'));
           assertAsignar((t_puntero) valorX, 3);
        } end

        it("ir a etiqueta") {
            analizadorLinea("goto inicio", funciones, kernel);
            assertIrAlLabel("inicio");
        } end

        it("alocar") {
            analizadorLinea("alocar x 6666 ", funciones, kernel);
                t_puntero punteroAlocar = assertMalloc(6666);
                t_puntero posicionX = assertObtenerPosicion('x');
                assertAsignar(posicionX, punteroAlocar);
        } end

        it("liberar") {
            analizadorLinea("liberar x", funciones, kernel);
            t_valor_variable valorX = assertDereferenciar(assertObtenerPosicion('x'));
            assertLiberar(valorX);
        } end

        it("liberar una posicion") {
            analizadorLinea("liberar &x", funciones, kernel);
            t_valor_variable posicionX = assertObtenerPosicion('x');
            assertLiberar(posicionX);
        } end

        it("liberar se puede operar como cualquier asignacion") {
            analizadorLinea("liberar x+5", funciones, kernel);
            t_valor_variable valorX = assertDereferenciar(assertObtenerPosicion('x'));
            assertLiberar(valorX + 5);
        } end

        it("imprimir un valor") {
            analizadorLinea("prints n x+53-&b", funciones, kernel);
                t_valor_variable valorX = assertDereferenciar(assertObtenerPosicion('x'));
                t_puntero posicionB = assertObtenerPosicion('b');
                char *valorAImprimir = string_itoa(valorX + 53 - posicionB);
                assertEscribir(DESCRIPTOR_SALIDA, valorAImprimir, string_length(valorAImprimir)+1);
        } end

        it("imprimir un valor de una variable compartida") {
                analizadorLinea("prints n !colas", funciones, kernel);
                t_valor_variable valorColas = assertObtenerValorCompartida("colas");
                char *valorAImprimir = string_itoa(valorColas);
                assertEscribir(DESCRIPTOR_SALIDA, valorAImprimir, string_length(valorAImprimir)+1);
        } end

        it("imprimir un literal") {
            analizadorLinea("prints l Holitas", funciones, kernel);
            assertEscribir(DESCRIPTOR_SALIDA, "Holitas", 8);
        } end

        it("imprimir un string en memoria") {
            //setup un dereferenciar de mentira
            funciones->AnSISOP_obtenerPosicionVariable = ({
                t_puntero __obtenerDeMentira(t_nombre_variable _){
                    return 0;
                } __obtenerDeMentira; });
            funciones->AnSISOP_dereferenciar = ({
                t_valor_variable __dereferenciarMentira(t_puntero puntero){
                    char stringEnMemoria[] = { 'H', 'o', 'l', 'a', '\0' };
                    return stringEnMemoria[puntero];
                } __dereferenciarMentira; });

            analizadorLinea("prints s &x", funciones, kernel);
            assertEscribir(DESCRIPTOR_SALIDA, "Hola", 5);

            //Rollback
            funciones->AnSISOP_obtenerPosicionVariable = obtenerPosicionVariable;
            funciones->AnSISOP_dereferenciar = dereferenciar;
        } end

         it("abrir un archivo") {
            analizadorLinea("abrir a LC /utn/so/archivo", funciones, kernel);
            t_descriptor_archivo fdAbierto = assertAbrir("/utn/so/archivo", (t_banderas){.lectura = true, .escritura = false, .creacion = true});
            t_puntero posicionA = assertObtenerPosicion('a');
            assertAsignar(posicionA, fdAbierto);
         } end

         it("abrir un archivo, los flags importan") {
            analizadorLinea("abrir a LE /utn/so/archivo", funciones, kernel);
            assertAbrir("/utn/so/archivo", (t_banderas){.lectura = true, .escritura = true, .creacion = false});
         } end

         it("borrar un archivo") {
            analizadorLinea("borrar t", funciones, kernel);
            t_puntero posicionT = assertObtenerPosicion('t');
            t_valor_variable descriptor = assertDereferenciar(posicionT);
            assertBorrar(descriptor);
         } end

         it("borrar un descriptor cualquiera") {
             analizadorLinea("borrar 5+2", funciones, kernel);
             assertBorrar(5+2);
         } end

         it("cerrar un descriptor") {
             analizadorLinea("cerrar 0", funciones, kernel);
             assertCerrar(0);
         } end

         it("desplazarse en un archivo") {
             analizadorLinea("buscar 3 t+5", funciones, kernel);
             t_valor_variable valorT = assertDereferenciar(assertObtenerPosicion('t'));
             assertMoverCursor(3, valorT+5);
         } end

         it("leer de un archivo") {
             analizadorLinea("leer 1 t 4-2", funciones, kernel);
             t_puntero valorT = assertDereferenciar(assertObtenerPosicion('t'));
             assertLeer(1, valorT, 4-2);
         } end

         it("leer de un archivo con fd que sea una variable") {
             analizadorLinea("leer a t 2", funciones, kernel);
             t_puntero valorT = assertDereferenciar(assertObtenerPosicion('t'));
             t_descriptor_archivo valorA = assertDereferenciar(assertObtenerPosicion('a'));
             assertLeer(valorA, valorT, 2);
         } end

         it("escribir en un archivo") {
             funciones->AnSISOP_obtenerPosicionVariable = ({
               t_puntero __obtenerDeMentira(t_nombre_variable _){
                  return 0;
               } __obtenerDeMentira; });
             funciones->AnSISOP_dereferenciar = ({
                t_valor_variable __dereferenciarMentira(t_puntero puntero){
                  char stringEnMemoria[] = { 'b', 'a', 'm', '\0', '\0' };
                  return stringEnMemoria[puntero];
                } __dereferenciarMentira; });

             analizadorLinea("escribir 0 &t 3", funciones, kernel);
             assertEscribir(0, "bam", 3);

             //Rollback
             funciones->AnSISOP_obtenerPosicionVariable = obtenerPosicionVariable;
             funciones->AnSISOP_dereferenciar = dereferenciar;
         } end

         it("escribir en un archivo con un fd variable") {
             analizadorLinea("escribir a+3 &t 0", funciones, kernel);
             t_valor_variable valorT = assertDereferenciar(assertObtenerPosicion('t'));
             t_valor_variable valorA = assertDereferenciar(assertObtenerPosicion('a'));
             assertEscribir(valorA+3, 0, 0);
         } end
    } end

    parserUtilTearDown();

}
