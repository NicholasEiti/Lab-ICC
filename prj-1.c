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

/// @brief Cuida de toda a leitura referente ao cadastro de usuário
int client_input(struct cliente *new_cliente){
	char usr_in[150];
	fgets(usr_in, 150, stdin);
	usr_in[strcspn(usr_in, "\n")] = 0;
	errno = 0;

	char* token;
	char* rest = usr_in;

	// Pega e trata cada um dos dados do cliente
	for(int i = 0; i < 3; i++){
		// Verifica se ainda há dados válidos entre as vírgulas
		if(!(token = strtok_r(rest, ",", &rest))){
			fprintf(stderr, "Erro: Falta de dados!\n");
			return 0;
		}
		char *endptr;
		int temp;
		switch (i){
			case 0:
				strcpy(new_cliente->nome, token);
				break;
			case 1:
				temp = strtol(token, &endptr, 10);
				// Verificação de validade da idade (quando o endptr não é '\0', 
				// há caracteres que não tem valor numérico
				if(temp < 0 || *endptr != '\0'){
					fprintf(stderr, "Erro: idade inválida\n");
					return 0;
				}
				new_cliente->idade = (unsigned int)temp;
			case 2:
				new_cliente->saldo = strtof(token, &endptr);
				if(new_cliente->saldo < 0 || *endptr != '\0'){
					fprintf(stderr, "Erro: saldo inválida\n");
					return 0;
				}
				break;
			default:
				break;
		}
	}

	// Verificar se ainda há informação sobrando na leitura das informações
	if(rest[0] != '\0'){
		fprintf(stderr, "Erro: Excesso de argumentos!\n");
		return 0;
	}
	return 1;
}

int transfer_input(int *id_orig, int *id_dest, float *quant){
	char usr_in[50];
	fgets(usr_in, 50, stdin);
	usr_in[strcspn(usr_in, "\n")] = 0;
	errno = 0;

	char* token;
	char* rest = usr_in;

	// Pega e trata cada um dos dados da entrada
	for(int i = 0; i < 3; i++){
		// Verifica se ainda há dados válidos entre as vírgulas
		if(!(token = strtok_r(rest, ",", &rest))){
			fprintf(stderr, "Erro: Falta de dados!\n");
			return 0;
		}
		char *endptr;
		int temp;
		float ftemp;
		switch (i){
			case 0:
				temp = strtol(token, &endptr, 10);
				// Verificação de validade da idade (quando o endptr não é '\0', 
				// há caracteres que não tem valor numérico
				if(*endptr != '\0'){
					fprintf(stderr, "Erro: id inválido\n");
					return 0;
				}
				*id_orig = temp;
				break;
			case 1:
				temp = strtol(token, &endptr, 10);
				// Verificação de validade da idade (quando o endptr não é '\0', 
				// há caracteres que não tem valor numérico
				if(*endptr != '\0'){
					fprintf(stderr, "Erro: id inválido\n");
					return 0;
				}		
				*id_dest = temp;			
				break;
			case 2:
				ftemp = strtof(token, &endptr);
				if(ftemp <= 0 || *endptr != '\0'){
					fprintf(stderr, "Erro: saldo inválido\n");
					return 0;
				}
				*quant = ftemp;
				break;
			default:
				break;
		}
	}

	// Verificar se ainda há informação sobrando na leitura das informações
	if(rest[0] != '\0'){
		fprintf(stderr, "Erro: Excesso de argumentos!\n");
		return 0;
	}
	return 1;
}

int main(int argc, char **argv[]){
	setlocale(LC_ALL,"pt-BR.UTF-8");
	print_menu();

	initialize_list();

	while(1){
		char op;
		loopstart:
		fputs("\nInsira uma opção: ", stdout);
		op = getchar();
		if(op == '\n'){
			continue;
		}
		getchar();

		int n_in;
		char **in;
		switch(op){
			case '0':
				// Sair e criar arquivo .txt
				puts("Saindo do sistema e criando um arquivo .txt");
				return 0;
			case '1':
				printf("");
				struct cliente new_cliente;
				
				if(client_input(&new_cliente)){
					create_user(new_cliente);
				}
				break;
			case '2':
				if(!scanf("%d", &n_in)){
					errno = 0;
					fprintf(stdin, "Valor inválido!\n");
					break;
				}
				getchar();

				struct cliente *new_clientes;
				new_clientes = (struct cliente *)malloc(n_in*sizeof(struct cliente));

				for (int i = 0; i < n_in; i++){
					struct cliente new_cliente;
					if(client_input(&new_cliente)){
						new_clientes[i] = new_cliente;
					}
					else{
						goto loopstart;
					}
				}
				create_users(new_clientes, n_in);
				break;
			case '3':
				if(!scanf("%d", &n_in)){
					errno = 0;
					fprintf(stderr, "Valor inválido!\n");
					break;
				}
				getchar();
				struct cliente c;
				c = find_user(n_in);
				if(c.id != -1){
					printf("ID: %d | Nome: %s | Idade: %u | Saldo: R$%.2f\n", c.id, c.nome, c.idade, c.saldo);
				}
				break;
			case '4':
				printf("");
				int id_orig;
				int id_dest;
				float quant;
				if(transfer_input(&id_orig, &id_dest, &quant)){
					transfer(id_orig, id_dest, quant);
				}
				break;
			case '5':
				if(!scanf("%d", &n_in)){
					errno = 0;
					fprintf(stderr, "Valor inválido!\n");
					break;
				}
				getchar();
				delete_user(n_in);
				break;
			case '6':
				list_all_users();
				break;
			case 'h':
				print_help();
				break;
			default:
				errno = 0;
				fprintf(stderr, "Erro: Opção inserida inválida!\n");
				break;
		}
	}
}
