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

#include "parser.h"

bool _string_contiene(char* aguja, char* pajar);
char* _string_trim(char* texto);
bool _esDefinicionVariable(char* linea);
bool _esDefinicionFuncion(char* linea);
bool _esVacio(char* linea);
bool _esDefinicionLabel(char* linea);
bool _esLlamadaFuncion(char* linea);
bool _esGoTo(char* linea);
bool _esLlamadaSignal(char* linea);
bool _esLlamadaWait(char* linea);
bool _esSaltoNZ(char* linea);
bool _esSaltoZ(char* linea);
bool _esAsignacion(char* linea);
bool _esEntradaSalida(char* linea);
bool _esImprimirVariable(char* linea);
bool _esImprimirTexto(char* linea);
bool _esFin(char* linea);
bool _esRetorno(char* linea);
bool _esEspacio(char caracter);
bool _separarDelimitadorYHacer(char* linea, t_puntero (*operacion_mandar)(t_nombre_variable));
void _saltoCondicional(char* linea, AnSISOP_funciones* como, bool(*comparacion)(t_valor_variable));
char* *_separarOperadores(char *texto, char *separador);
t_puntero _obtenerPosicion(t_nombre_variable* operador, AnSISOP_funciones* como);
t_valor_variable _operar(char* operador, AnSISOP_funciones* como);
char** _obtenerParametros(char* params, t_valor_variable* parametrosValor, AnSISOP_funciones *operations);
void _llamadaFuncion(char* parametrosLiteral, void(*helper_llamada)(void), AnSISOP_funciones* como);

void analizadorLinea(char* const instruccion, AnSISOP_funciones *AnSISOP_funciones, AnSISOP_kernel *AnSISOP_funciones_kernel){
	char	*linea,
			*lineaTemporal = linea = strdup( instruccion );
	linea = strdup( _string_trim(linea) );
	free(lineaTemporal);


	if( _esFin(linea) ){
		AnSISOP_funciones->AnSISOP_finalizar();
	} else 	if( _esDefinicionVariable(linea)){
		_separarDelimitadorYHacer(linea, AnSISOP_funciones->AnSISOP_definirVariable);
	} else if( _esAsignacion(linea) ){
		char* *operation = _separarOperadores(linea, TEXT_ASSIGNATION);

		if(operation[0][0] == TEXT_VAR_START_GLOBAL)
			AnSISOP_funciones->AnSISOP_asignarValorCompartida( operation[0]+1,
														 _operar(operation[1], AnSISOP_funciones)
														);
		else if(operation[0][0] == TEXT_DEREFERENCE_OP)											//Si la parte izquirda empieza con *
			AnSISOP_funciones->AnSISOP_asignar(	_operar(operation[0]+1, AnSISOP_funciones),	//Queres _operar
														_operar(operation[1], AnSISOP_funciones)
													);
		else
			AnSISOP_funciones->AnSISOP_asignar(	_obtenerPosicion(operation[0], AnSISOP_funciones),
											_operar(operation[1], AnSISOP_funciones)
										);

		free(operation);
	} else if( _esImprimirVariable(linea) ){
		AnSISOP_funciones->AnSISOP_imprimir( _operar(_string_trim(linea + strlen(TEXT_PRINT)), AnSISOP_funciones) );
	} else if( _esImprimirTexto(linea) ){
		AnSISOP_funciones->AnSISOP_imprimirTexto(linea + strlen(TEXT_PRINT_TEXT));	//No trimeo, proque imprime el literal
	} else if( _esRetorno(linea) ){
		AnSISOP_funciones->AnSISOP_retornar( _operar(_string_trim(linea + strlen(TEXT_RETURN)), AnSISOP_funciones)  );
	} else if( _esGoTo(linea) ){
		AnSISOP_funciones->AnSISOP_irAlLabel(_string_trim(linea + strlen(TEXT_GOTO)));
	} else if( _esSaltoNZ(linea) ){
		bool __comparacionNZ(t_valor_variable x){	return x != 0;	}
		_saltoCondicional(linea, AnSISOP_funciones, __comparacionNZ);
	} else if( _esSaltoZ(linea) ){
		bool __comparacionZ(t_valor_variable x){	return x == 0;	}
		_saltoCondicional(linea, AnSISOP_funciones, __comparacionZ);
	} else if( _esLlamadaSignal(linea) ){
		AnSISOP_funciones_kernel->AnSISOP_signal( _string_trim(linea + strlen(TEXT_SIGNAL)) );
	} else if( _esLlamadaWait(linea) ){
		AnSISOP_funciones_kernel->AnSISOP_wait( _string_trim(linea + strlen(TEXT_WAIT)) );
	} else if( _esEntradaSalida(linea) ){
		char* *operation = string_split(linea + strlen(TEXT_IO), " ");
		AnSISOP_funciones->AnSISOP_entradaSalida( _string_trim(operation[0]), atoi(_string_trim(operation[1])) );
		string_iterate_lines(operation, (void*)free);
		free(operation);
	} else if( _esLlamadaFuncion(linea) ){
		//RETORNO <- ETIQUETA PARAMETROS
		char* *ret = _separarOperadores(linea, TEXT_CALL);
		char* *lineAux = _separarOperadores(ret[1], " ");
		void __conRetorno(void){
			AnSISOP_funciones->AnSISOP_llamarConRetorno( lineAux[0], _obtenerPosicion(ret[0], AnSISOP_funciones));
		}
		_llamadaFuncion(lineAux[1], __conRetorno, AnSISOP_funciones);
		free(lineAux);
		free(ret);
	} else {
		char* *lineAux = _separarOperadores(linea, " ");
		void __sinRetorno(void){
			AnSISOP_funciones->AnSISOP_llamarSinRetorno( lineAux[0] );
		}
		_llamadaFuncion(lineAux[1], __sinRetorno, AnSISOP_funciones);
		free(lineAux);
	}
	free(linea);
}


// **** FUNCIONES AUXILIARES ****


bool _string_contiene(char* aguja, char* pajar){
	return strstr(aguja, pajar) != NULL;
}

char* _string_trim(char* texto){
    int i;
    while (_esEspacio (*texto)) texto++;   //Anda a el primer no-espacio
    for (i = strlen (texto) - 1; i>0 && (_esEspacio (texto[i])); i--);   //y de atras para adelante
    texto[i + 1] = '\0';
    return texto;
}

bool _esDefinicionVariable(char* linea){
	return string_starts_with(linea, TEXT_VARIABLE);
}

bool _esDefinicionFuncion(char* linea){
	return string_starts_with(linea, TEXT_FUNCTION);
}

bool _esVacio(char* linea){
	return !(linea[0] == TEXT_COMMENT || linea[0] == '\n');
}

bool _esDefinicionLabel(char* linea){
	return string_starts_with(linea, TEXT_START_LABEL);
}

bool _esLlamadaFuncion(char* linea){
	return _string_contiene(linea, TEXT_CALL);
}

bool _esGoTo(char* linea){
	return string_starts_with(linea, TEXT_GOTO);
}

bool _esLlamadaSignal(char* linea){
	return string_starts_with(linea, TEXT_SIGNAL);
}

bool _esLlamadaWait(char* linea){
	return string_starts_with(linea, TEXT_WAIT);
}

bool _esSaltoNZ(char* linea){
	return string_starts_with(linea, TEXT_JUMPNZ);
}

bool _esSaltoZ(char* linea){
	return string_starts_with(linea, TEXT_JUMPZ);
}

bool _esAsignacion(char* linea){
	 return _string_contiene(linea, TEXT_ASSIGNATION);
}

bool _esEntradaSalida(char* linea){
	 return _string_contiene(linea, TEXT_IO);
}

bool _esImprimirVariable(char* linea){
	return string_starts_with(linea, TEXT_PRINT);
}

bool _esImprimirTexto(char* linea){
	return string_starts_with(linea, TEXT_PRINT_TEXT);
}

bool _esFin(char* linea){
	return string_starts_with(linea, TEXT_END);
}

bool _esRetorno(char* linea){
	return string_starts_with(linea, TEXT_RETURN);
}

bool _esEspacio(char caracter){
	return caracter==' ' || caracter=='\t' || caracter=='\f' || caracter=='\r' || caracter=='\v';
}

bool _separarDelimitadorYHacer(char* linea, t_puntero (*operacion_mandar)(t_nombre_variable)){
	char* lista_variables = _string_trim(linea + strlen(TEXT_VARIABLE));
	char* *variables = string_split(lista_variables, TEXT_DELIM);

	void __helperSend(char* valor){
		operacion_mandar( _string_trim(valor)[0] );
	}

	string_iterate_lines(variables, __helperSend);
	string_iterate_lines(variables, (void*)free);
	free(variables);
	return true;
}

void _saltoCondicional(char* linea, AnSISOP_funciones* como, bool(*comparacion)(t_valor_variable)){
	char* *operation = string_split(linea, " ");
	if( comparacion(_operar(_string_trim(operation[1]), como)) )
		como->AnSISOP_irAlLabel( _string_trim(operation[2]) );
	string_iterate_lines(operation, (void*)free);
	free(operation);
}

char* *_separarOperadores(char *texto, char *separador) {
	char* *ret = malloc(2 * sizeof(char*) );
	char* offset = strstr(texto, separador);
	if( offset != NULL ){	//Si encuentra el substring
		*offset = '\0'; 	//Dividir el string
		ret[0] = _string_trim(texto);
		ret[1] = _string_trim(offset + strlen(separador));
	} else {
		ret[0] = texto;
		ret[1] = NULL;
	}
	return ret;
}

t_puntero _obtenerPosicion(t_nombre_variable* operador, AnSISOP_funciones* como){
	if( operador[0] == TEXT_VAR_START_PARAM){
		return como->AnSISOP_obtenerPosicionVariable(operador[1]);
	}
	return como->AnSISOP_obtenerPosicionVariable(operador[0]);

}

t_valor_variable _operar(char* operador, AnSISOP_funciones* como){
	if( operador[0] == TEXT_DEREFERENCE_OP)		//Como son las primeras cosas que se evaluan, para hacer "x=(*p)+3" tenemos que hacer: "a=*p; x+3"
		return como->AnSISOP_dereferenciar( _operar(operador+1, como) );	//Llamada recursiva antes de evaluar


	t_valor_variable ret;
	if( _string_contiene(operador, TEXT_SUM) ){
		char* *subOperations = _separarOperadores(operador, TEXT_SUM);
		ret = _operar(subOperations[0], como) + _operar(subOperations[1], como);
		free(subOperations);
		return ret;
	} else if( _string_contiene(operador, TEXT_SUBSTRACTION) ){
		char* *subOperations = _separarOperadores(operador, TEXT_SUBSTRACTION);
		ret = _operar(subOperations[0], como) - _operar(subOperations[1], como);
		free(subOperations);
		return ret;
	}	//Si no es suma ni resta, dame el valor como variable
	if( operador[0] == TEXT_REFERENCE_OP)
		return _obtenerPosicion(operador+1, como);	//Sin el '&'
					//No los dejo operar despues del "&"
	if( operador[0] >= '0' && operador[0] <= '9' )	//Si es un numerito
		return atoi(operador);
	if( operador[0] == TEXT_VAR_START_GLOBAL )
		return como->AnSISOP_obtenerValorCompartida( operador+1 );

	return como->AnSISOP_dereferenciar(_obtenerPosicion(operador, como));
}

char** _obtenerParametros(char* params, t_valor_variable* parametrosValor, AnSISOP_funciones *operations){
	char* *parametros;
	int i;

	if(params == NULL)	//Si no tiene parametros
		return NULL;		//Devolver null

	parametros = string_split(params, TEXT_DELIM);

	//Guardar el valor de los parametros en el vector
	for( i=0; parametros[i] != NULL; i++){
		parametrosValor[i] = _operar(parametros[i], operations);
	}

	return parametros;
}

void _llamadaFuncion(char* parametrosLiteral, void(*helper_llamada)(void), AnSISOP_funciones* como){
	t_valor_variable parametrosValor[10];	//10 parametros maximo (0 al 9)
	char* *parametros = _obtenerParametros( parametrosLiteral, parametrosValor, como );

	helper_llamada();

	//Definir y asignar los parametos "anonimos" despues del cambio de contexto de la llamada
	if( parametros != NULL ){
		int i;
		for( i=0; parametros[i] != NULL; i++)
			como->AnSISOP_asignar( como->AnSISOP_definirVariable( '0'+i ),	//el literal 0, mas la posicion me da el literal i
											parametrosValor[i] );

		string_iterate_lines(parametros, (void *)free);
		free(parametros);
	}
}
