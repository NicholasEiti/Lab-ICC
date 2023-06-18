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
		fprintf(stderr, "Linha muito grande!");
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
            fprintf(stderr, "Erro: Excesso de argumentos!\n");
            return 0;
        }
        args[i] = token;
		token = strtok(NULL, delim);
		i++;
	}
	if(i < argc){
		fprintf(stderr, "Erro: Falta de dados!\n");
		return 0;
	}

	
	return 1;
}

int client_input(struct cliente *new_cliente){
	char** usr_in;
	usr_in = malloc(NOME_LEN+20);
	if(!(line_input(stdin, 3, NOME_LEN+20, ",", usr_in))){
		return 0;
	}

	if(strlen(usr_in[0]) > NOME_LEN){
		fprintf(stderr, "Erro: nome deve ter no maximo 100 caracteres");
		return 0;
	}
	strcpy(new_cliente->nome, usr_in[0]);

	char *endptr;
	int temp = strtol(usr_in[1], &endptr, 10);
	// Verificação de validade da idade (quando o endptr não é '\0', 
	// há caracteres que não tem valor numérico
	if(temp < 0 || *endptr != '\0'){
		fprintf(stderr, "Erro: idade invalida\n");
		return 0;
	}
	new_cliente->idade = (unsigned int)temp;

	new_cliente->saldo = strtof(usr_in[2], &endptr);
	if(new_cliente->saldo < 0 || *endptr != '\0'){
		new_cliente->saldo = 0;
		fprintf(stderr, "Erro: saldo invalido\n");
		return 0;
	}
	return 1;
}

int client_data(char *nome, char *idade, char *saldo, struct cliente *new_cliente){
	if(strlen(nome) > NOME_LEN){
		fprintf(stderr, "Erro: nome deve ter no maximo 100 caracteres");
		return 0;
	}
	strcpy(new_cliente->nome, nome);

	char *endptr;
	int temp = strtol(idade, &endptr, 10);
	// Verificação de validade da idade (quando o endptr não é '\0', 
	// há caracteres que não tem valor numérico
	if(temp < 0 || *endptr != '\0'){
		fprintf(stderr, "Erro: idade invalida\n");
		return 0;
	}
	new_cliente->idade = (unsigned int)temp;

	new_cliente->saldo = strtof(saldo, &endptr);
	if(new_cliente->saldo < 0 || *endptr != '\0'){
		new_cliente->saldo = 0;
		fprintf(stderr, "Erro: saldo invalido\n");
		return 0;
	}
	return 1;
}

int transfer_input(char *id_orig, char *id_dest, float *quant){
	char** usr_in;
	usr_in = malloc(30 * sizeof(char));
	if(!(line_input(stdin, 3, 30, " ", usr_in))){
		free(usr_in);
		return 0;
	}

	if(strlen(usr_in[0]) != 6){
		fprintf(stderr, "ID invalido!\n");
		return 0;
	}
	strcpy(id_orig, usr_in[0]);
	id_orig = usr_in[0];

	if(strlen(usr_in[1]) != 6){
		fprintf(stderr, "ID invalido!\n");
		free(usr_in);
		return 0;
	}
	strcpy(id_dest, usr_in[1]);
	id_dest = usr_in[1];

	char *endptr;

	float ftemp = strtof(usr_in[2], &endptr);
	if(*endptr != '\0'){
		fprintf(stderr, "Erro: saldo invalido\n");
		free(usr_in);
		return 0;
	}
	*quant = ftemp;

	free(usr_in);
	
	return 1;
}