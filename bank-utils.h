#include <stdio.h>
#include <uuid/uuid.h>

#ifndef BANK_UTILS_
#define BANK_UTILS_

#define NOME_LEN 100

struct cliente{
    uuid_t id;
    char nome[100];
    unsigned int idade;
    float saldo;
};

extern struct cliente cliente_vazio;

// Inicializa a lista de clientes (esvazia ela)
void initialize_list();

// Imprime todos os clientes no terminal
void list_all_users();

// Imprime os dados de um cliente (não necessáriamente na lista)
void print_user_data(struct cliente c);

// Cria um relatório da lista de clientes cadastrados
void report();

// Carrega um relatório de lista de clientes
int load_report(char* filename);

// Adiciona um cliente na lista de clientes
int create_user(struct cliente novo_cliente);

// Adiciona vários clientes na lista de clientes
int create_users(struct cliente novos_clientes[], int size);

// Retorna o usuário com o id do argumento; retorna cliente_vazio caso o cliente não seja encontrado
struct cliente find_user(char trg_id[6]);

// Realiza a transferência entre dois usuários
int transfer(char id_orig[6], char id_dest[6], float quant);

// Deleta um usuário com o id do argumento
int delete_user(char trg_id[6]);

// Processa uma linha de entrada
int line_input(FILE* stream, int argc, int size, char* delim, char** args);

// Processa uma linha de input para dados de cliente
int client_input(struct cliente *new_cliente);

// Processa os dados apresentados
int client_data(char *nome, char *idade, char *saldo, struct cliente *new_cliente);

// Processa linha dos dados de transferência
int transfer_input(char *id_orig, char *id_dest, float *quant);

#endif
