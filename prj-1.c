#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <uuid/uuid.h>
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

void list_all_users(struct cliente lista[100]){
	for(int i = 0; i < 100; i++){
		if(!uuid_is_null(lista[i].id)){
			printf("Usuário ID: %s", lista[i].id);
			printf(" Nome: %s", lista[i].nome);
			printf(" Idade: %d", lista[i].idade);
			printf(" Saldo: R$%.2f\n", lista[i].saldo);
		}
	}
}

int main(){
	struct cliente clientes[100];
	setlocale(LC_ALL,"pt-BR.UTF-8");
	print_menu();
	while(1){
		char opt_in[100];
		char usr_in[200];
		fputs("Insira uma opção: ", stdout);
		fgets(opt_in, 100, stdin);

		if(strlen(opt_in) > 2){
			opt_in[0] = 'x';
		}
		switch(opt_in[0]){
			case '0':
				// Sair e criar arquivo .txt
				puts("Saindo do sistema e criando um arquivo .txt");
				return 0;
			case '1':
				// (clientes, "Jailson", 10, 1000);
				printf("Usuário inserido com id - com sucesso\n");
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
				list_all_users(clientes);
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
