#ifndef BANK_UTILS_
#define BANK_UTILS_
struct cliente
{
    int id;
    char nome[100];
    unsigned int idade;
    float saldo;
};

int create_user(struct cliente lista[100], char nome[100], unsigned int idade, float saldo);

int find_user(struct cliente lista[100], int id);

int transfer(struct cliente lista[100], int id_orig, int id_dest, float quant);

int delete_user(struct cliente lista[100], int id);

#endif
