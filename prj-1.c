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

/// @brief Cuida de toda a leitura referente ao cadastro de usuário
int client_input(struct cliente *new_cliente){
	char** usr_in;
	usr_in = malloc(NOME_LEN+20);
	if(!(line_input(3, NOME_LEN+20, ",", usr_in))){
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

int transfer_input(int *id_orig, int *id_dest, float *quant){
	char** usr_in;
	usr_in = malloc(30);
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
	if(ftemp <= 0 || *endptr != '\0'){
		fprintf(stderr, "Erro: saldo invalido\n");
		return 0;
	}
	*quant = ftemp;
	
	return 1;
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
					printf("ID: %d | Nome: %s | Idade: %u | Saldo: R$%.2f\n", c.id, c.nome, c.idade, c.saldo);
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
