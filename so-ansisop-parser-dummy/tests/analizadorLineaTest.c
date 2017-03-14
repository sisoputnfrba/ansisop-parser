#include <parser/parser.h>
#include <cspecs/cspec.h>
#include "utils/parserUtils.h"

AnSISOP_funciones *funciones = NULL;
AnSISOP_kernel *kernel = NULL;

context (parser) {

    void setup(){
        funciones = malloc(sizeof(AnSISOP_funciones));
        kernel = malloc(sizeof(AnSISOP_kernel));
        parserUtilSetup();

        funciones->AnSISOP_definirVariable = definirVariable;
        funciones->AnSISOP_obtenerPosicionVariable = obtenerPosicionVariable;
        funciones->AnSISOP_dereferenciar = dereferenciar;
        funciones->AnSISOP_asignar = asignar;

        kernel->AnSISOP_alocar = alocar;
    };

    setup();

    describe("Si al parser") {
        it("definicion de variables") {
            analizadorLinea("variables x, a, g", funciones, kernel);
                assertDefinirVariable('x');
                assertDefinirVariable('a');
                assertDefinirVariable('g');
        } end
        it("asignacion de variables") {
            analizadorLinea("x = a+3", funciones, kernel);
                assertObtenerPosicion('a');
                assertDereferenciar(ultimoRetorno()->puntero);
                assertObtenerPosicion('x');
                assertAsignar(ultimoRetorno()->puntero, ultimoRetorno()->valor_variable+3);
        } end

        it("alocar") {
            analizadorLinea("alocar x 6666 ", funciones, kernel);
                assertMalloc(6000);
                assertObtenerPosicion('x');
                t_puntero posicionX = ultimoRetorno()->puntero;
                assertAsignar(posicionX, ultimoRetorno()->valor_variable);
        } end
    } end

}