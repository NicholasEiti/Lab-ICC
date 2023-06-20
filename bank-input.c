#include "bank-utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int line_input(FILE* stream, int argc, int size, char* delim, char** args){
    char *usr_in = NULL;
	size_t buffer_size = 0;

	getline(&usr_in, &buffer_size, stream);
	setbuf(stream, NULL);
	
	usr_in[strcspn(usr_in, "\n")] = 0;

	if(strlen(usr_in) > size){
		fputs("Linha muito grande!\n", stderr);
		return 0;
	}

	errno = 0;

	char* token;
	char* rest = usr_in;
	
	token = strtok(rest, delim);
	int i = 0;
	while (token != NULL){
        if(i >= argc){
			errno = 7;
            fputs("Erro: Excesso de argumentos!\n", stderr);
            return 0;
        }
        args[i] = token;
		token = strtok(NULL, delim);
		i++;
	}
	if(i < argc){
		fputs("Erro: Falta de dados!\n", stderr);
		return 0;
	}

	
	return 1;
}

int client_input(struct cliente *new_cliente){
	char** usr_in;
	usr_in = (char **)malloc(NOME_LEN+20);
	if(!(line_input(stdin, 3, NOME_LEN+20, ",", usr_in))){
		free(usr_in);
		return 0;
	}

	if(strlen(usr_in[0]) > NOME_LEN){
		fputs("Erro: nome deve ter no maximo 100 caracteres\n", stderr);
		free(usr_in);
		return 0;
	}
	strcpy(new_cliente->nome, usr_in[0]);

	char *endptr;
	int temp = strtol(usr_in[1], &endptr, 10);
	// Verificação de validade da idade (quando o endptr não é '\0', 
	// há caracteres que não tem valor numérico
	if(temp < 0 || *endptr != '\0'){
		fputs("Erro: idade invalida\n", stderr);
		free(usr_in);
		return 0;
	}
	new_cliente->idade = (unsigned int)temp;

	new_cliente->saldo = strtof(usr_in[2], &endptr);
	if(new_cliente->saldo < 0 || *endptr != '\0'){
		new_cliente->saldo = 0;
		fputs("Erro: saldo invalido\n", stderr);
		free(usr_in);
		return 0;
	}
	free(usr_in);
	return 1;
}

int client_data(char *nome, char *idade, char *saldo, struct cliente *new_cliente){
	if(strlen(nome) > NOME_LEN){
		fputs("Erro: nome deve ter no maximo 100 caracteres\n", stderr);
		return 0;
	}
	strcpy(new_cliente->nome, nome);

	char *endptr;
	int temp = strtol(idade, &endptr, 10);
	// Verificação de validade da idade (quando o endptr não é '\0', 
	// há caracteres que não tem valor numérico
	if(temp < 0 || *endptr != '\0'){
		fputs("Erro: idade invalida\n", stderr);
		return 0;
	}
	new_cliente->idade = (unsigned int)temp;

	new_cliente->saldo = strtof(saldo, &endptr);
	if(new_cliente->saldo < 0 || *endptr != '\0'){
		new_cliente->saldo = 0;
		fputs("Erro: saldo invalido\n", stderr);
		return 0;
	}
	return 1;
}

int transfer_input(char *id_orig, char *id_dest, float *quant){
	char** usr_in;
	usr_in = (char **)malloc(30 * sizeof(char));
	if(!(line_input(stdin, 3, 30, " ", usr_in))){
		free(usr_in);
		return 0;
	}

	if(strlen(usr_in[0]) != 6){
		fputs("Erro: ID de origem invalido!\n", stderr);
		free(usr_in);
		return 0;
	}
	strcpy(id_orig, usr_in[0]);
	id_orig = usr_in[0];

	if(strlen(usr_in[1]) != 6){
		fputs("Erro: ID de destino invalido!\n", stderr);
		free(usr_in);
		return 0;
	}
	strcpy(id_dest, usr_in[1]);
	id_dest = usr_in[1];

	char *endptr;

	float ftemp = strtof(usr_in[2], &endptr);
	if(*endptr != '\0'){
		fputs("Erro: saldo invalido!\n", stderr);
		free(usr_in);
		return 0;
	}
	*quant = ftemp;

	free(usr_in);
	return 1;
}