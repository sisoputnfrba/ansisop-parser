#ifndef ANSISOP_PARSER_PARSERUTILS_H
#define ANSISOP_PARSER_PARSERUTILS_H

    #include <parser/parser.h>
    #include <cspecs/cspec.h>

    #define remplazar(funcion, retorno, cuerpo) \
        funciones->AnSISOP_##funcion = ({       \
            retorno __fn__ cuerpo               \
            __fn__;                             \
        })

#endif //ANSISOP_PARSER_PARSERUTILS_H
