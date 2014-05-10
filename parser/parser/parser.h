/*
 * Copyright (C) 2012 Sistemas Operativos - UTN FRBA. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PARSER_H_
#define PARSER_H_

	#include "sintax.h"

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>

	#include <commons/collections/dictionary.h>
	#include <commons/string.h>
	#include <commons/string.h>
	#include <commons/error.h>

	//Tipos de datos
	typedef u_int32_t t_puntero;
	typedef u_int32_t t_size;
	typedef u_int32_t t_puntero_instruccion;

	typedef char t_nombre_variable;
	typedef int t_valor_variable;

	typedef t_nombre_variable* t_nombre_semaforo;
	typedef t_nombre_variable* t_nombre_etiqueta;
	typedef  t_nombre_variable* t_nombre_compartida;
	typedef  t_nombre_variable* t_nombre_dispositivo;

	typedef enum {
		SIN_ERROR,
		NO_EXISTE_VARIABLE,
		NO_EXISTE_FUNCION,
		NO_EXISTE_ETIQUETA,
		STACK_OVERFLOW,
		STACK_UNDERFLOW,
		FIN_PROGRAMA,
	} STATUS;

	//Operaciones
	typedef struct {
		/*
		 * DEFINIR VARIABLE
		 *
		 * Reserva en el Contexto de Ejecución Actual el espacio necesario para una variable llamada identificador_variable y la registra tanto en el Stack como en el Diccionario de Variables. Retornando la posición del valor de esta nueva variable del stack
		 * El valor de la variable queda indefinido: no deberá inicializarlo con ningún valor default.
		 * Esta función se invoca una vez por variable, a pesar que este varias veces en una línea.
		 * Ej: Evaluar "variables a, b, c" llamará tres veces a esta función con los parámetros "a", "b" y "c"
		 *
		 * @sintax	TEXT_VARIABLE (variables identificador[,identificador]*)
		 * @param	identificador_variable	Nombre de variable a definir
		 * @return	Puntero a la variable recien asignada
		 */
		t_puntero (*AnSISOP_definirVariable)(t_nombre_variable identificador_variable);

		/*
		 * OBTENER POSICION de una VARIABLE
		 *
		 * Devuelve el desplazamiento respecto al inicio del segmento Stacken que se encuentra el valor de la variable identificador_variable del contexto actual.
		 * En caso de error, retorna -1.
		 *
		 * @sintax	TEXT_REFERENCE_OP (&)
		 * @param	identificador_variable 		Nombre de la variable a buscar (De ser un parametro, se invocara sin el '$')
		 * @return	Donde se encuentre la variable buscada
		 */
		t_puntero (*AnSISOP_obtenerPosicionVariable)(t_nombre_variable identificador_variable);

		/*
		 * DEREFERENCIAR
		 *
		 * Obtiene el valor resultante de leer a partir de direccion_variable, sin importar cual fuera el contexto actual
		 *
		 * @sintax	TEXT_DEREFERENCE_OP (*)
		 * @param	direccion_variable	Lugar donde buscar
		 * @return	Valor que se encuentra en esa posicion
		 */
		t_valor_variable (*AnSISOP_dereferenciar)(t_puntero direccion_variable);

		/*
		 * ASIGNAR
		 *
		 * Inserta una copia del valor en la variable ubicada en direccion_variable.
		 *
		 * @sintax	TEXT_ASSIGNATION (=)
		 * @param	direccion_variable	lugar donde insertar el valor
		 * @param	valor	Valor a insertar
		 * @return	void
		 */
		void (*AnSISOP_asignar)(t_puntero direccion_variable, t_valor_variable valor);

		/*
		 * OBTENER VALOR de una variable COMPARTIDA
		 *
		 * Pide al kernel el valor (copia, no puntero) de la variable compartida por parametro.
		 *
		 * @sintax	TEXT_VAR_START_GLOBAL (!)
		 * @param	variable	Nombre de la variable compartida a buscar
		 * @return	El valor de la variable compartida
		 */
		t_valor_variable (*AnSISOP_obtenerValorCompartida)(t_nombre_compartida variable);

		/*
		 * ASIGNAR VALOR a variable COMPARTIDA
		 *
		 * Pide al kernel asignar el valor a la variable compartida.
		 * Devuelve el valor asignado.
		 *
		 * @sintax	TEXT_VAR_START_GLOBAL (!) IDENTIFICADOR TEXT_ASSIGNATION (=) EXPRESION
		 * @param	variable	Nombre (sin el '!') de la variable a pedir
		 * @param	valor	Valor que se le quire asignar
		 * @return	Valor que se asigno
		 */
		t_valor_variable (*AnSISOP_asignarValorCompartida)(t_nombre_compartida variable, t_valor_variable valor);


		/*
		 * IR a la ETIQUETA
		 *
		 * Cambia la linea de ejecucion a la correspondiente de la etiqueta buscada.
		 *
		 * @sintax	TEXT_GOTO (goto )
		 * @param	t_nombre_etiqueta	Nombre de la etiqueta
		 * @return	void
		 */
		void (*AnSISOP_irAlLabel)(t_nombre_etiqueta t_nombre_etiqueta);

		/*
		 * LLAMAR SIN RETORNO
		 *
		 * Preserva el contexto de ejecución actual para poder retornar luego al mismo.
		 * Modifica las estructuras correspondientes para mostrar un nuevo contexto vacío.
		 *
		 * Los parámetros serán definidos luego de esta instrucción de la misma manera que una variable local, con identificadores numéricos empezando por el 0.
		 *
		 * @sintax	Sin sintaxis particular, se invoca cuando no coresponde a ninguna de las otras reglas sintacticas
		 * @param	etiqueta	Nombre de la funcion
		 * @return	void
		 */
		void (*AnSISOP_llamarSinRetorno)(t_nombre_etiqueta etiqueta);

		/*
		 * LLAMAR CON RETORNO
		 *
		 * Preserva el contexto de ejecución actual para poder retornar luego al mismo, junto con la posicion de la variable entregada por donde_retornar.
		 * Modifica las estructuras correspondientes para mostrar un nuevo contexto vacío.
		 *
		 * Los parámetros serán definidos luego de esta instrucción de la misma manera que una variable local, con identificadores numéricos empezando por el 0.
		 *
		 * @sintax	TEXT_CALL (<-)
		 * @param	etiqueta	Nombre de la funcion
		 * @param	donde_retornar	Posicion donde insertar el valor de retorno
		 * @return	void
		 */
		void (*AnSISOP_llamarConRetorno)(t_nombre_etiqueta etiqueta, t_puntero donde_retornar);


		/*
		 * FINALIZAR
		 *
		 * Cambia el Contexto de Ejecución Actual para volver al Contexto anterior al que se está ejecutando, recuperando el Cursor de Contexto Actual y el Program Counter previamente apilados en el Stack.
		 * En caso de estar finalizando el Contexto principal (el ubicado al inicio del Stack), deberá finalizar la ejecución del programa.
		 *
		 * @sintax	TEXT_END (end )
		 * @param	void
		 * @return	void
		 */
		void (*AnSISOP_finalizar)(void);

		/*
		 * RETORNAR
		 *
		 * Cambia el Contexto de Ejecución Actual para volver al Contexto anterior al que se está ejecutando, recuperando el Cursor de Contexto Actual, el Program Counter y la direccion donde retornar, asignando el valor de retorno en esta, previamente apilados en el Stack.
		 *
		 * @sintax	TEXT_RETURN (return )
		 * @param	retorno	Valor a ingresar en la posicion corespondiente
		 * @return	void
		 */
		void (*AnSISOP_retornar)(t_valor_variable retorno);

		/*
		 * IMPRIMIR
		 *
		 * Envía valor_mostrar al Kernel, para que termine siendo mostrado en la consola del Programa en ejecución.
		 *
		 * @sintax	TEXT_PRINT (print )
		 * @param	valor_mostrar	Dato que se quiere imprimir
		 * @return	void
		 */
		void (*AnSISOP_imprimir)(t_valor_variable valor_mostrar);

		/*
		 * IMPRIMIR TEXTO
		 *
		 * Envía mensaje al Kernel, para que termine siendo mostrado en la consola del Programa en ejecución. mensaje no posee parámetros, secuencias de escape, variables ni nada.
		 *
		 * @sintax TEXT_PRINT_TEXT (textPrint )
		 * @param	texto	Texto a imprimir
		 * @return void
		 */
		void (*AnSISOP_imprimirTexto)(char* texto);

		/*
		 *	ENTRADA y SALIDA
		 *
		 *
		 *	@sintax TEXT_IO (io )
		 *	@param	dispositivo	Nombre del dispositivo a pedir
		 *	@param	tiempo	Tiempo que se necesitara el dispositivo
		 *	@return	void
		 */
		void (*AnSISOP_entradaSalida)(t_nombre_dispositivo dispositivo, int tiempo);
	} AnSISOP_funciones;

	//Operaciones de Kernel
	typedef struct {
		/*
		 * WAIT
		 *
		 * Informa al kernel que ejecute la función wait para el semáforo con el nombre identificador_semaforo.
		 * El kernel deberá decidir si bloquearlo o no.
		 *
		 * @sintax	TEXT_WAIT (wait )
		 * @param	identificador_semaforo	Semaforo a aplicar WAIT
		 * @return	void
		 */
		void (*AnSISOP_wait)(t_nombre_semaforo identificador_semaforo);

		/*
		 * SIGNAL
		 *
		 * Informa al kernel que ejecute la función signal para el semáforo con el nombre identificador_semaforo.
		 * El kernel deberá decidir si desbloquear otros procesos o no.
		 *
		 * @sintax	TEXT_SIGNAL (signal )
		 * @param	identificador_semaforo	Semaforo a aplicar SIGNAL
		 * @return	void
		 */
		void (*AnSISOP_signal)(t_nombre_semaforo identificador_semaforo);
	} AnSISOP_kernel;

	void analizadorLinea(char* const instruccion, AnSISOP_funciones *AnSISOP_funciones, AnSISOP_kernel *AnSISOP_funciones_kernel);
	char* _string_trim(char*);

#endif
