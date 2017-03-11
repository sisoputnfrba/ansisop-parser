#include <parser/parser.h>
#include <cspecs/cspec.h>
#include "utils/parserUtils.h"

context (parser) {

    AnSISOP_funciones *funciones = NULL;
    AnSISOP_kernel *kernel = NULL;

    void limpiarFunciones(){
        if(funciones != NULL)
            free(funciones);
        if(kernel != NULL)
            free(kernel);

        funciones = malloc(sizeof(AnSISOP_funciones));
        kernel = malloc(sizeof(AnSISOP_kernel));
    }


    describe("Si al parser") {
        before {
            limpiarFunciones();
        } end

        after {
            limpiarFunciones();
        } end

        it("le das una linea es de terminar, entonces ejecuta la funcion de finalizar") {
            remplazar(finalizar, void, (void){
                    //Ok!
            });
            analizadorLinea("end", funciones, kernel);
        } end
    } end

}