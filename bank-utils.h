#ifndef BANK_UTILS_
#define BANK_UTILS_
#include <uuid/uuid.h>

struct cliente
{
    uuid_t id;
    char nome[100];
    int idade;
    float saldo;
};

int create_user(struct cliente lista[100], char nome[100], int idade, float saldo);

int find_user(struct cliente lista[100], uuid_t id);

int transfer(struct cliente lista[100], uuid_t id_orig, uuid_t id_dest, float quant);

int delete_user(struct cliente lista[100], uuid_t id);

#endif
