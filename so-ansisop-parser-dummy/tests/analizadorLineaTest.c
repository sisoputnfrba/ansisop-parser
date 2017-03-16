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
        funciones->AnSISOP_irAlLabel = irAlLabel;
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
                t_valor_variable acumulador = ultimoRetorno()->valor_variable;
                assertAsignar(ultimoRetorno()->puntero, acumulador+3);
        } end

        it("ir a etiqueta") {
            analizadorLinea("goto inicio", funciones, kernel);
            assertIrAlLabel("inicio");
        } end
    } end

}
