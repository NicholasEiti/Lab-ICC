#include "bank-utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int line_input(int argc, int size, char* delim, char** args){
    char usr_in[size];
	fgets(usr_in, size, stdin);
	setbuf(stdin, NULL);
	usr_in[strcspn(usr_in, "\n")] = 0;
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
	if(!(line_input(3, NOME_LEN+20, ",", usr_in))){
		return 0;
	}
	
	if(strlen(usr_in[0]) > NOME_LEN){
		errno = 0;
		fprintf(stderr, "Aviso: nome muito grande!");
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

int transfer_input(int *id_orig, int *id_dest, float *quant){
	char** usr_in;
	usr_in = malloc(30 * sizeof(char));
	if(!(line_input(3, 30, " ", usr_in))){
		return 0;
	}

	char* endptr;

	int temp = strtol(usr_in[0], &endptr, 10);
	// Verificação de validade do id (quando o endptr não é '\0', 
	// há caracteres que não tem valor numérico
	if(*endptr != '\0'){
		fprintf(stderr, "Erro: id de origem invalido\n");
		return 0;
	}
	*id_orig = temp;

	temp = strtol(usr_in[1], &endptr, 10);
	// Verificação de validade da idade (quando o endptr não é '\0', 
	// há caracteres que não tem valor numérico
	if(*endptr != '\0'){
		fprintf(stderr, "Erro: id de destino invalido\n");
		return 0;
	}		
	*id_dest = temp;

	float ftemp = strtof(usr_in[2], &endptr);
	if(*endptr != '\0'){
		fprintf(stderr, "Erro: saldo invalido\n");
		return 0;
	}
	*quant = ftemp;
	
	return 1;
}