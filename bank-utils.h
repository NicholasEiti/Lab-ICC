#ifndef BANK_UTILS_
#define BANK_UTILS_

#define NOME_LEN 100

struct cliente
{
    int id;
    char nome[100];
    unsigned int idade;
    float saldo;
};

void initialize_list();

void list_all_users();

void print_user_data(struct cliente c);

void report();

int create_user(struct cliente novo_cliente);

int create_users(struct cliente novos_clientes[], int size);

struct cliente find_user(int id);

int transfer(int id_orig, int id_dest, float quant);

int delete_user(int id);

int client_input(struct cliente *new_cliente);

int transfer_input(int *id_orig, int *id_dest, float *quant);

#endif
