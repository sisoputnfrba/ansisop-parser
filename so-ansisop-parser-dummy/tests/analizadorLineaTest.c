#include <stdbool.h>
#include <parser/parser.h>
#include <cspecs/cspec.h>

context (parser) {

    AnSISOP_funciones *funciones = NULL;
    AnSISOP_kernel *kernel = NULL;

    void limpiarFunciones(){
        if(funciones != NULL)
            free(funciones);
        if(kernel  != NULL)
            free(kernel);

        funciones = malloc(sizeof(AnSISOP_funciones));
        kernel = malloc(sizeof(AnSISOP_kernel));

        funciones->AnSISOP_finalizar = ({
            void __fn__ (void) {
                fail("No se deberia estar llamando a esta funcion");
            } __fn__;
        });
    }


    describe("Si al parser") {
        before {
            limpiarFunciones();
        } end

        after {
            limpiarFunciones();
        } end

        it("le das una linea es de terminar, entonces ejecuta la funcion de finalizar") {

            analizadorLinea("end", funciones, kernel);
        } end
    } end

}