#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "bank-utils.h"

void print_menu(){
	puts("------------MENU UAI BANK------------");
	puts("0- Sair");
	puts("1- Inserção de um novo usuário");
	puts("2- Inserção de vários usuários");
	puts("3- Busca de um usuário por id");
	puts("4- Transferência entre usuários");
	puts("5- Remoção de usuário");
	puts("h- Exibição do guia de ajuda");
}

void print_help(){
	puts("\n------------GUIA UAI BANK------------");
	puts("0- Sai do menu e cria um arquivo .txt da base de usuários");
	puts("1- <nome>, <idade>, <saldo atual>");
	puts("Insere um usuário com nome, idade e saldo\n");
	puts("2- <qtde usuários>");
	puts("<nome>, <idade>, <saldo atual>(de cada usuário)");
	puts("Insere uma quantidade <qtde usuários> de usuários com nome, idade e saldo\n");
	puts("3- <id>");
	puts("Realiza uma busca de um usuário com id <id>\n");
	puts("4- <id de origem> <id de destino> <quantia>");
	puts("Transferência entre usuário <id de origem> para <id de destino>\n");
	puts("5- <id>");
	puts("Remove usuário com id <id>\n");
}

// Adicionar função para ler as entradas dos clientes por meio de pointers
// Se o id fosse 
int client_input(char nome[], unsigned int *idade, float *saldo){
	char usr_in[150];
	fgets(usr_in, 150, stdin);
	strtok(usr_in, "\n"); // Remove o caractere \n do input
	errno = 0;

	char* token;
	char* rest = usr_in;

	// Pega e trata cada uma dos dados do cliente
	for(int i = 0; i < 3; i++){
		if((token = strtok_r(rest, ",", &rest))){
			char *endptr;
			int temp;
			switch (i){
				case 0:
					strcpy(nome, token);
					break;
				case 1:
					temp = strtol(token, &endptr, 10);
					// Verificação de validade da idade (quando o endptr não é '\0', 
					// há caracteres que não tem valor numérico
					if(temp < 0 || *endptr != '\0'){
						fprintf(stderr, "Erro: idade inválida\n");
						return 0;
					}
					*idade = (unsigned int)temp;
				case 2:
					*saldo = strtof(token, &endptr);
					if(*saldo < 0 || *endptr != '\0'){
						fprintf(stderr, "Erro: saldo inválida\n");
						return 0;
					}
					break;
				default:
					break;
			}
		}
		else{
			fprintf(stderr, "Erro: Falta de dados!\n");
			return 0;
		}
	}

	// Verificar se ainda há informação sobrando na leitura das informações
	if(rest[0] != '\0'){
		fprintf(stderr, "Erro: Excesso de argumentos!\n");
		return 0;
	}
	return 1;
}

int main(){
	struct cliente clientes[100];
	setlocale(LC_ALL,"pt-BR.UTF-8");
	print_menu();

	while(1){
		char op;
		fputs("Insira uma opção: ", stdout);
		op = getchar();

		getchar(); // Capturar enter
		switch(op){
			case '0':
				// Sair e criar arquivo .txt
				puts("Saindo do sistema e criando um arquivo .txt");
				return 0;
			case '1':
				printf("");
				char nome[100];
				unsigned int idade;
				float saldo;
				if(client_input(nome, &idade, &saldo)){
					printf("Usuário inserido com id - com sucesso\n");
					printf("%s %u %.2f\n", nome, idade, saldo);
					create_user(clientes, nome, idade, saldo);
				}
				
				break;
			case '2':
				// create_user();
				printf("Usuários inseridos com id - com sucesso\n");
				break;
			case '3':
				// find_user();
				break;
			case '4':
				// transfer();
				break;
			case '5':
				// remove_user();
				break;
			case '6':
				break;
			case 'h':
				print_help();
				break;
			default:
				puts("Erro: Opção inserida inválida!\n\n");
				break;
		}
	}
}
