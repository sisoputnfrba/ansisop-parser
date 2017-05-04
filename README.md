# AnSISOP Parser

![AnSISOP](http://i.imgur.com/joNONHL.png)
[![Build Status](https://travis-ci.org/sisoputnfrba/ansisop-parser.svg?branch=master)](https://travis-ci.org/sisoputnfrba/ansisop-parser)

En este repositorio encontrarán el analizador de linea del lenguaje inventado **AnSISOP**, junto con herramientas útiles para obtener meta data de un programa, a ser utilizados en trabajos prácticos como "*Está CoverFlow*", "*Elestac*", entre otros.

La documentación completa de cada función a implementar se encuentra en [documentación completa](parser/parser/parser.h)

## Biblioteca compartida **parser**
La biblioteca cuenta con dos grandes funcionalidades:

1. `analizadorLinea`: El *parser* propiamente, que dada una linea te texto literal, desifra que primitivas llamar
2. `metadatada_desde_literal`: El pre-procesador del codigo

### Guía de Instalación
1. Instalar la biblioteca compartida *commons* ([Guía de Instalación](https://github.com/sisoputnfrba/so-commons-library#gu%C3%ADa-de-instalaci%C3%B3n))
2. `cd parser`: Ir al directorio de la biblioteca compartida
3. `make all`: Compilar la biblioteca compartida *(de no utilizar nuestro makefile, recordar linkear con la biblioteca de commons)*
4. `ls build/`: Revisar que exista el archivo *libparser-ansisop.so* dentro del directorio `build/`
5. `sudo make install`: Instala la biblioteca, para que todos tengan acceso a la misma

   `sudo make uninstall`: Desinstala la biblioteca

### Guía para el uso:
Linkear con `-lparser-ansisop`, o lo que es lo mismo, desde Eclipse:

1. Click derecho sobre el proyecto en el *Proyect Explorer*
2. *Properties*
3. *C/C++ General*
4. *Paths and Symbols*
5. *Libraries*
6. *Add...*
7. Escribir `parser-ansisop` (sin el *-l* delante)

Para usarla en un .c/.h debería incluirse de la siguiente forma: `parser/parser.h` o `parser/metadata_program.h`

Por ejemplo:

```c
#include <parser/metadata_program.h>
```

#### Andamiaje para el uso:
Dentro de este mismo repositorio se encuentra el proyecto de [dummy-cpu](dummy-cpu) que sive como una primera aproximacion del desarollo de la CPU

## Tests

Para ejecutar los tests simplemente hacer `make test` dentro del subdirectorio de `parset-tests`
