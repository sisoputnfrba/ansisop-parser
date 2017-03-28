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
		 * NUMBER_TO_ASCII
		 *
		 * Convierte el contenido de un valor numerico a ASCII para luego ser impreso.
		 *
		 * @JOACO La idea de esto es que pueda ser llamado para la funcion writeNumber que imprime numeros en un archivo. Confirma que tan viable es, sino hay que crear una syscall con la misma firma que Write, pero que imprima numeros en uan variable y no un offset a partir de un puntero.
		 * 
		 * @sintax	WRITE_NUMBER (writeNumber ) 	Convierte este numero a un puntero y luego llama a la funcion Write
		 * @param	number		 		Numero a convertir
		 * @return	t_puntero 			Puntero que contiene el String convertido
		 */
		t_puntero (*AnSISOP_number_to_ascii)(t_valor_variable numero);


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

		/*
		 * MALLOC
		 *
		 * Informa al Kernel que el proceso requiere que se reserven size bytes de memoria dinamica para el mismo.
		 *
		 * @syntax 	MALLOC (malloc )
		 * @param	memoria_requerida	Cantidad de bytes que el programa requiere reservar.
		 * @return	Direccion de memoria en la cual comienza el espacio reservado.
		 */
		t_puntero (*AnSISOP_malloc)(t_valor_variable memoria_requerida);

		/*
		 * FREE
		 *
		 * Informa al Kernel que el proceso requiere que se liberen los bytes reservados por malloc. 
		 *
		 * @syntax 	MALLOC (malloc )
		 * @param	puntero		Puntero que contiene la posicion de inicio del bloque a liberar.
		 * @return	void
		 */
		void (*AnSISOP_free)(t_puntero puntero);

		/*
		 * OPEN
		 *
		 * Informa al Kernel que el proceso requiere que se abra un archivo.
		 *
		 * @syntax 	OPEN (open )
		 * @param	path		Ruta al archivo a abrir.	
		 * @param	flags		String que contiene los permisos con los que se abre el archivo.
		 * @return	t_file_descriptor	El valor del FD abierto por el sistema.
		 */
		t_file_descriptor (*AnSISOP_open)(t_path path, t_flags flags);

		/*
		 * DELETE
		 *
		 * Informa al Kernel que el proceso requiere que se borre un archivo.
		 *
		 * @syntax 	DELETE (delete )
		 * @param	path		Ruta al archivo a abrir.	
		 * @return	void
		 */
		t_file_descriptor (*AnSISOP_open)(t_path path);

		/*
		 * CLOSE
		 *
		 * Informa al Kernel que el proceso requiere que se cierre un archivo.
		 *
		 * @syntax 	CLOSE (close )
		 * @param	file_descriptor		File Descriptor del archivo abierto.
		 * @return	void
		 */
		void (*AnSISOP_close)(t_file_descriptor file_descriptor);


		/*
		 * WRITE 
		 *
		 * Informa al Kernel que el proceso requiere que se escriba un archivo.
		 *
		 * @syntax 	WRITE (write )
		 * @param	file_descriptor		File Descriptor del archivo abierto.
		 * @param	data			Puntero que indica donde comienza la informacion a ser copiada.
		 * @param	size			Tamanio de la informacion a enviar. Debe ser un offset valido de putero. 
		 * @return	void
		 */
		void (*AnSISOP_write)(t_file_descriptor file_descriptor, t_puntero data, t_valor_variable size);


	} AnSISOP_kernel;

	void analizadorLinea(char* const instruccion, AnSISOP_funciones *AnSISOP_funciones, AnSISOP_kernel *AnSISOP_funciones_kernel);
	char* _string_trim(char*);

#endif
