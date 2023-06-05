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

int create_user(struct cliente novo_cliente);

int create_users(struct cliente novos_clientes[], int size);

struct cliente find_user(int id);

int transfer(int id_orig, int id_dest, float quant);

int delete_user(int id);

int line_input(int argc, int size, char* delim, char** args);

#endif
