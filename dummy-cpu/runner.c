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

#include <stdio.h>
#include <commons/log.h>
#include <string.h>
#include <parser/metadata_program.h>

#include "dummy_ansisop.h"


static const char* PROGRAMA =
		"begin\n"
		"variables a, b\n"
		"a = 3\n"
		"b = 5\n"
		"a = b + 12\n"
		"end\n"
		"\n";

AnSISOP_funciones functions = {
		.AnSISOP_definirVariable		= dummy_definirVariable,
		.AnSISOP_obtenerPosicionVariable= dummy_obtenerPosicionVariable,
		.AnSISOP_finalizar 				= dummy_finalizar,
		.AnSISOP_dereferenciar			= dummy_dereferenciar,
		.AnSISOP_asignar				= dummy_asignar,

};
AnSISOP_kernel kernel_functions = { };

char *const conseguirDatosDeLaMemoria(char *start, t_puntero_instruccion offset, t_size i);

int main(int argc, char **argv) {
	printf("Ejecutando\n");
	char *programa = strdup(PROGRAMA);
	t_metadata_program *metadata = metadata_desde_literal(programa);

	int programCounter = 0;
	while(!terminoElPrograma()){
		char* const linea = conseguirDatosDeLaMemoria(programa,
													  metadata->instrucciones_serializado[programCounter].start,
													  metadata->instrucciones_serializado[programCounter].offset);
		printf("\t Evaluando -> %s", linea);
		analizadorLinea(linea, &functions, &kernel_functions);
		free(linea);
		programCounter++;
	}
	metadata_destruir(metadata);
	printf("================\n");

	return EXIT_SUCCESS;
}

char *const conseguirDatosDeLaMemoria(char *prgrama, t_puntero_instruccion inicioDeLaInstruccion, t_size tamanio) {
	char *aRetornar = calloc(1, 100);
	memcpy(aRetornar, prgrama + inicioDeLaInstruccion, tamanio);
	return aRetornar;
}

