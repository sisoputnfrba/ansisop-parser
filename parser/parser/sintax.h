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

#ifndef SINTAX_H_
#define SINTAX_H_

	#define LINE_SIZE 100
	#define PROGRAM_LINES 100
	#define PROGRAM_SIZE LINE_SIZE*PROGRAM_LINES
	#define CODIGO_FIN -1

	//Diccionario:
	#define TEXT_VARIABLE "variables "
	#define TEXT_DELIM ","
	#define TEXT_FUNCTION "function "

	#define TEXT_PRINT "print "
	#define TEXT_PRINT_TEXT "textPrint "
	#define TEXT_CALL "<-"
	#define TEXT_GOTO "goto "

	#define TEXT_BEGIN "begin"
	#define TEXT_END "end"
	#define TEXT_RETURN "return"

	#define TEXT_WAIT "wait "
	#define TEXT_SIGNAL "signal "

	#define TEXT_ASSIGNATION "="
	#define TEXT_SUM "+"
	#define TEXT_SUBSTRACTION "-"

	#define TEXT_START_LABEL ":"
	#define TEXT_COMMENT '#'

	#define TEXT_JUMPNZ "jnz "
	#define TEXT_JUMPZ "jz "

	#define TEXT_IO "io "

	#define TEXT_VAR_START_PARAM '$'
	#define TEXT_VAR_START_GLOBAL '!'
	#define TEXT_DEREFERENCE_OP '*'
	#define TEXT_REFERENCE_OP '&'

#endif
