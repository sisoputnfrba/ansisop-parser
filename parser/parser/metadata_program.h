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

#ifndef METADATA_PROGRAM_H_
#define METADATA_PROGRAM_H_

	#include "parser.h"
	#include "sintax.h"

	#include <stdlib.h>
	#include <sys/stat.h>
	#include <stdlib.h>
	#include <string.h>

	#include <commons/string.h>

	typedef struct {
		t_puntero_instruccion	start;
		t_size		offset;
	} t_intructions;

	typedef struct {
		t_puntero_instruccion	instruccion_inicio;	//El numero de la primera instruccion (Begin)
		t_size			instrucciones_size;				// Cantidad de instrucciones
		t_intructions*	instrucciones_serializado; 		// Instrucciones del programa

		t_size			etiquetas_size;					// Tamaño del mapa serializado de etiquetas
		char*			etiquetas;							// La serializacion de las etiquetas

		int				cantidad_de_funciones;
		int				cantidad_de_etiquetas;
	} t_metadata_program;

t_metadata_program* metadata_desde_literal(const char*);
void metadata_destruir(t_metadata_program*);
t_puntero_instruccion metadata_buscar_etiqueta(const t_nombre_etiqueta objetivo, const char *etiquetas, const t_size etiquetas_size);

// compatibilidad hacia atras
t_metadata_program* metadatada_desde_literal(const char*);
typedef t_metadata_program t_medatada_program;

#endif
