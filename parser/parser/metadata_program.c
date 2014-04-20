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

#include "metadata_program.h"

char* _separarLineas(char*);
void _agregarEtiqueta(char*, int*, t_medatada_program*, char*);

t_medatada_program* metadatada_desde_literal(char* program_literal){
	t_medatada_program* ret = malloc( sizeof(t_medatada_program) );

	memset(ret, 0, sizeof(t_medatada_program) );
	char* buffer;

	int position = 0;			//Bytes de offset en el literal
	int etiqueteasSize = 0;	//No es cuantas etiquetas, sino el tamanio  fisico de las etiquetas
	int tamanioTotal = strlen(program_literal);

	_separarLineas(program_literal);
	while( position<tamanioTotal ){
		buffer = program_literal+position;
		t_intructions auxinst = { .start = position, .offset = strlen(buffer) +1 };
		buffer = _string_trim(buffer);

		if( string_starts_with(buffer, TEXT_BEGIN) ){
			ret->instruccion_inicio = ret->instrucciones_size;
		} else {
			if(	string_starts_with(buffer, TEXT_START_LABEL) ){
				_agregarEtiqueta(buffer, &etiqueteasSize, ret, TEXT_START_LABEL);
			} else if( string_starts_with(buffer, TEXT_FUNCTION) ){
				_agregarEtiqueta(buffer, &etiqueteasSize, ret, TEXT_FUNCTION);
			} else if( !(buffer[0] == TEXT_COMMENT || buffer[0] == '\0') ) {
					ret->instrucciones_size++;
					ret->instrucciones_serializado = realloc( ret->instrucciones_serializado, sizeof(t_intructions) * ret->instrucciones_size );
					memcpy( ret->instrucciones_serializado + ret->instrucciones_size - 1, &auxinst, sizeof(t_intructions) );	//La pone en el programa
			}
		}
		position += auxinst.offset;
	}
	return ret;
}

void metadata_destruir(t_medatada_program* victima){
	free(victima->etiquetas);
	free(victima->instrucciones_serializado);
	free(victima);
}

t_puntero_instruccion metadata_buscar_etiqueta(t_medatada_program* programa, t_nombre_etiqueta etiqueta){
	int i=0;
	int offset = 0;
	char* nombre;
	for(i=0; i < programa->etiquetas_size; i++){
		nombre = programa->etiquetas + offset;
		if( string_equals_ignore_case(nombre, etiqueta) )
			return *(nombre + 1 + strlen(nombre));
		offset += strlen(nombre) + 1 + sizeof(t_puntero_instruccion);
	}
	return -1;
}


// **** FUNCIONES AUXILIARES ****


char* _separarLineas(char* linea){
	int i;
	for(i=0; linea[i] != '\0'; i++){
		if( linea[i] == '\n' ){
			linea[i] = '\0';
		}
	}
	return linea;
}

void _agregarEtiqueta(char* linea, int* etiquetaSz, t_medatada_program* programa, char* prefix){
	programa->cantidad_de_funciones++;
	char* auxName = linea + strlen(prefix) ;
	int etiquetaNameLength = (strlen(auxName) +1) * sizeof(char);

	programa->etiquetas = realloc(programa->etiquetas, *etiquetaSz + etiquetaNameLength + sizeof(t_puntero_instruccion));
	memcpy(programa->etiquetas+*etiquetaSz, auxName, etiquetaNameLength );
	memcpy(programa->etiquetas+*etiquetaSz+etiquetaNameLength, &programa->instrucciones_size, sizeof(t_puntero_instruccion) );

	*etiquetaSz += etiquetaNameLength+sizeof(t_puntero_instruccion);
	programa->etiquetas_size++;
}
