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
	puts("6- Listar todos os clientes");
	puts("h- Exibir do guia de ajuda");
}

void print_help(){
	puts("\n------------GUIA UAI BANK------------");
	puts("0- Sai do menu e cria um arquivo .txt da base de clientes");
	puts("1- <nome>, <idade>, <saldo atual>");
	puts("Insere um cliente com nome, idade e saldo");
	puts("2- <qtde clientes>");
	puts("<nome>, <idade>, <saldo atual>(de cada cliente)");
	puts("Insere uma quantidade <qtde clientes> de clientes com nome, idade e saldo");
	puts("3- <id>");
	puts("Realiza uma busca de um cliente com id <id>");
	puts("4- <id de origem> <id de destino> <quantia>");
	puts("Transferência entre cliente <id de origem> para <id de destino>");
	puts("5- <id>");
	puts("Remove cliente com id <id>");
	puts("6- Lista toda a lista de clientes");
}

int main(int argc, char *argv[]){
	setlocale(LC_ALL,"pt-BR.UTF-8");

	initialize_list();
	if(argc == 2){
		if(!load_report(argv[1])){
			return 0;
		}
	}
	print_menu();
	while(1){
		loopstart:
		char op, aux;
		fputs("\nInsira uma opcao: ", stdout);
		op = getchar();
		aux = getchar();
		if(aux != '\n'){
			while(aux != '\n'){
				aux = getchar();
			}
			op = '\0';
		}
		int n_in;

		char *id_orig = malloc(6);
		char *id_dest = malloc(6);
		float quant;

		char *id_in = NULL;
		size_t buffer_size = 0;

		struct cliente new_cliente;

		switch(op){
			case '0': // Criar relatório
				puts("Saindo do sistema e criando relatorio");
				report();
				return 0;
			case '1': // Criar cliente
				if(client_input(&new_cliente)){
					create_user(new_cliente);
				}
				break;
			case '2': // Criar clientes
				if(!scanf("%d", &n_in)){
					errno = 0;
					fprintf(stderr, "Valor invalido!\n");
					setbuf(stdin, NULL);
					break;
				}
				setbuf(stdin, NULL);

				// Inicialização do array dos novos clientes a serem adicionados
				struct cliente *new_clientes;
				new_clientes = (struct cliente *)malloc(n_in*sizeof(struct cliente));

				// Leitura dos dados de cada novo cliente
				for (int i = 0; i < n_in; i++){
					struct cliente new_cliente;
					if(client_input(&new_cliente)){
						new_clientes[i] = new_cliente;
					}
					else{
						free(new_clientes);
						goto loopstart;
					}
				}
				
				create_users(new_clientes, n_in);
				free(new_clientes);
				break;
			case '3': // Busca de cliente
				getline(&id_in, &buffer_size, stdin);
				setbuf(stdin, NULL);
				id_in[strcspn(id_in, "\n")] = 0;
				if(strlen(id_in) != 6){
					fprintf(stderr, "ID invalido!\n");
					break;
				}

				struct cliente c;
				c = find_user(id_in);
				if(strcmp(c.id, "\0") != 0){
					print_user_data(c);
				}
				break;
			case '4': // Transferência entre clientes
				if(transfer_input(id_orig, id_dest, &quant)){
					if(transfer(id_orig, id_dest, quant)){
						puts("Transferencia realizada com sucesso.");
					}
					else{
						errno = 0;
        				fprintf(stderr, "Transferencia cancelada.\n");
					}
				}
				break;
			case '5': // Remover cliente
				getline(&id_in, &buffer_size, stdin);
				setbuf(stdin, NULL);
				id_in[strcspn(id_in, "\n")] = 0;
				if(strlen(id_in) != 6){
					fprintf(stderr, "ID invalido!");
					break;
				}

				delete_user(id_in);
				break;
			case '6': // Listagem dos clientes
				list_all_users();
				break;
			case 'h': // Ajuda
				print_help();
				break;
			default:
				errno = 0;
				fprintf(stderr, "Erro: Opcao inserida invalida!\n");
				break;
		}
	}
}
