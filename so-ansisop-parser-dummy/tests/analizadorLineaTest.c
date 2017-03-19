#include <parser/parser.h>
#include <cspecs/cspec.h>
#include "utils/parserUtils.h"

AnSISOP_funciones *funciones = NULL;
AnSISOP_kernel *kernel = NULL;
bool imprimirEnPantalla = false;

context (parser) {

    void setup(){
        funciones = malloc(sizeof(AnSISOP_funciones));
        kernel = malloc(sizeof(AnSISOP_kernel));
        parserUtilSetup(tmpnam(NULL), imprimirEnPantalla);

        funciones->AnSISOP_definirVariable = definirVariable;
        funciones->AnSISOP_obtenerPosicionVariable = obtenerPosicionVariable;
        funciones->AnSISOP_dereferenciar = dereferenciar;
        funciones->AnSISOP_asignar = asignar;
        funciones->AnSISOP_imprimirValor = imprimir;
        funciones->AnSISOP_imprimirLiteral = imprimirLiteral;

        kernel->AnSISOP_alocar = alocar;
        kernel->AnSISOP_liberar = liberar;
    };

    setup();

    before {
        limpiarElContextoDeEjecucion();
    } end

    describe("Si al parser") {
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

        it("alocar") {
            analizadorLinea("alocar x 6666 ", funciones, kernel);
                t_puntero punteroAlocar = assertMalloc(6666);
                t_puntero posicionX = assertObtenerPosicion('x');
                assertAsignar(posicionX, punteroAlocar);
        } end

        it("liberar") {
            analizadorLinea("liberar x", funciones, kernel);
                t_puntero posicionX = assertObtenerPosicion('x');
                assertLiberar(posicionX);
        } end

        it("imprimir un valor") {
            analizadorLinea("prints n x+53-&b", funciones, kernel);
                t_valor_variable valorX = assertDereferenciar(assertObtenerPosicion('x'));
                t_puntero posicionB = assertObtenerPosicion('b');
                assertImprimir(valorX+53-posicionB);
        } end

        it("imprimir un literal") {
            analizadorLinea("prints l Holitas", funciones, kernel);
                assertImprimirLiteral("Holitas");
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

            analizadorLinea("prints s x", funciones, kernel);
                assertImprimirLiteral("H");
                assertImprimirLiteral("o");
                assertImprimirLiteral("l");
                assertImprimirLiteral("a");

            //Rollback
            funciones->AnSISOP_obtenerPosicionVariable = obtenerPosicionVariable;
            funciones->AnSISOP_dereferenciar = dereferenciar;
        } end
    } end

    parserUtilTearDown();

}