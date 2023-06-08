#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "bank-utils.h"

void print_menu(){
	puts("------------MENU UAI BANK------------");
	puts("0- Sair");
	puts("1- Inserir um novo clientes");
	puts("2- Inserir varios clientes");
	puts("3- Buscar um cliente por id");
	puts("4- Transferir entre clientes");
	puts("5- Remover clientes");
	puts("h- Exibir do guia de ajuda");
}

void print_help(){
	puts("\n------------GUIA UAI BANK------------");
	puts("0- Sai do menu e cria um arquivo .txt da base de clientes");
	puts("1- <nome>, <idade>, <saldo atual>");
	puts("Insere um cliente com nome, idade e saldo\n");
	puts("2- <qtde clientes>");
	puts("<nome>, <idade>, <saldo atual>(de cada cliente)");
	puts("Insere uma quantidade <qtde clientes> de clientes com nome, idade e saldo\n");
	puts("3- <id>");
	puts("Realiza uma busca de um cliente com id <id>\n");
	puts("4- <id de origem> <id de destino> <quantia>");
	puts("Transferência entre cliente <id de origem> para <id de destino>\n");
	puts("5- <id>");
	puts("Remove cliente com id <id>\n");
}

int main(int argc, char **argv[]){
	setlocale(LC_ALL,"pt-BR.UTF-8");
	print_menu();

	initialize_list();

	while(1){
		char op;
		loopstart:
		fputs("\nInsira uma opcao: ", stdout);
		op = getchar();
		if(op == '\n'){
			continue;
		}
		getchar();

		int n_in;
		switch(op){
			case '0':
				// Sair e criar arquivo .txt
				puts("Saindo do sistema e criando relatorio");
				report();
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
					fprintf(stdin, "Valor invalido!\n");
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
				free(new_clientes);
				break;
			case '3':
				if(!scanf("%d", &n_in)){
					errno = 0;
					fprintf(stderr, "Valor invalido!\n");
					break;
				}
				getchar();
				struct cliente c;
				c = find_user(n_in);
				if(c.id != -1){
					print_user_data(c);
				}
				break;
			case '4':
				printf("");
				int id_orig;
				int id_dest;
				float quant;
				if(transfer_input(&id_orig, &id_dest, &quant)){
					if(transfer(id_orig, id_dest, quant)){
						puts("Transferencia realizada com sucesso.");
					}
					else{
						errno = 0;
        				fprintf(stderr, "Transferencia cancelada.\n");
					}
				}
				break;
			case '5':
				if(!scanf("%d", &n_in)){
					errno = 0;
					fprintf(stderr, "Valor invalido!\n");
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
				fprintf(stderr, "Erro: Opcao inserida invalida!\n");
				break;
		}
	}
}
