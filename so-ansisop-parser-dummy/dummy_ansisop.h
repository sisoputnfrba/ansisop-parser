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


#ifndef DUMMY_ANSISOP_H_
#define DUMMY_ANSISOP_H_

	#include <parser/parser.h>

	#include <stdio.h>

	t_puntero dummy_definirVariable(t_nombre_variable variable);

	t_puntero dummy_obtenerPosicionVariable(t_nombre_variable variable);
	t_valor_variable dummy_dereferenciar(t_puntero puntero);
	void dummy_asignar(t_puntero puntero, t_valor_variable variable);

	void dummy_imprimir(t_valor_variable valor);
	void dummy_imprimirTexto(char* texto);

#endif

