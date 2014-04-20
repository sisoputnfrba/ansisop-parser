# AnSISOP Parser
=================

![AnSISOP](http://i.imgur.com/joNONHL.png)

En este repositorio encontrarán el analizador de linea del lenguaje inventado **[AnSISOP](http://ansisop.florius.com.ar)**, junto con herramientas útiles para obtener meta data de un programa, a ser utilizados en el trabajo práctico "*Está CoverFlow*" de Sistemas Operativos, 1<sup>er</sup> cuatrimestre, 2014.

La documentación completa de cada función a implementar se encuentra en [documentación completa](parser/parser/parser.h#L55-L260)

## Guía de compilación

#### Biblioteca compartida **parser**
1. `cd parser`: Ir al directorio de la biblioteca compartida
2. `make all`: Compilar la biblioteca compartida
3. `ls build/`: Revisar que exista el archivo *libparser-ansisop.so* dentro del directorio `build/`
4. `make install`: Instala la biblioteca, para que todos tengan acceso a la misma, agregando como parametro de compilacion `-lparser-ansisop`

#### Ejemplo de implementación
1. `cd so-ansisop-parser-dummy`: Ir al directorio del ejemplo
2. `make all`: Compilar el ejemplo
3. `ls build/`: Revisar que exista el archivo *so-ansisop-parser-dummy* dentro del directorio `build/`
