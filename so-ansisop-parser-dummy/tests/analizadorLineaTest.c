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
    };

    setup();

    describe("Si al parser") {
        it("XXXXXX") {
            analizadorLinea("variables x, a, g", funciones, kernel);
                Llamada* llamada = ultimaLlamada();
                should_string(llamada->nombre) be equal to("definirVariable");
                should_char(llamada->parametros[0].nombre_variable) be equal to('x');
        } end
    } end

}