#include "bank-utils.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

unsigned int counter = 0;
unsigned int id = 0;

struct cliente lista_clientes[100];

const struct cliente cliente_vazio = { -1, "", 0, 0 };

void initialize_list(){
    for(int i = 0; i < 100; i++){
        lista_clientes[i] = cliente_vazio;
    }
}

void list_all_users(){
    for (int i = 0; i < 100; i++){
        if(lista_clientes[i].id != -1){
            printf("ID: %d | Nome: %s | Idade: %u | Saldo: R$%.2f\n", 
            lista_clientes[i].id, lista_clientes[i].nome, lista_clientes[i].idade, lista_clientes[i].saldo);
        }
    }
}

/// @brief Cria um usuário com id, nome, idade e saldo
int create_user(struct cliente novo_cliente){
    lista_clientes[counter] = novo_cliente;
    lista_clientes[counter].id = ++id;
    printf("Usuário inserido com id %d com sucesso\n", id);
    counter++;
    return 1;
}

int create_users(struct cliente novos_clientes[], int size){
    printf("Usuários inseridos com id ");
    for(int i = 0; i < size; i++){
        lista_clientes[counter] = novos_clientes[i];
        lista_clientes[counter].id = ++id;
        printf("%d", id);
        if(i != size-1){
            printf(",");
        }
        counter++;
    }
    printf(" com sucesso\n");
}

/// @brief Acha usuário a partir do id
/// @param lista a lista de clientes
/// @param id identificação do cliente
/// @return Retorna o index do usuário em referência a lista; retorna -1 caso o usuário não seja encontrado/inválido
struct cliente find_user(int id){
    for(int i = 0; i < 100; i++){
        if(lista_clientes[i].id == -1) continue;
        if(lista_clientes[i].id == id){
            return lista_clientes[i];
        }
    }
    errno = 0;
    fprintf(stderr, "Cliente com id %d inesistente!\n", id);
    return cliente_vazio;
}

struct cliente *find_user_ptr(int id){
    for(int i = 0; i < 100; i++){
        if(lista_clientes[i].id == -1) continue;
        if(lista_clientes[i].id == id){
            return &lista_clientes[i];
        }
    }
    errno = 0;
    fprintf(stderr, "Cliente com id %d inesistente!\n", id);
    return NULL;
}

/// @brief Transferência entre dois usuários
/// @param id_orig id da origem da transferência
/// @param id_dest id do alvo da transferência
/// @param quant valor da transferência
/// @return Retorna 1 caso a transferência tenha sucesso; retorna 0 caso um dos usuários não seja encontrado; 
/// retorna -1 caso a transferência seja inválida por outro motivo
int transfer(int id_orig, int id_dest, float quant){
    // Achar o usuário id_orig
    struct cliente *org = find_user_ptr(id_orig);
    if(org->id == cliente_vazio.id){ return 0; }

    // Achar o usuário id_dest
    struct cliente *dest = find_user_ptr(id_orig);
    if(dest->id == cliente_vazio.id){ return 0; }

    if(org->saldo < quant){
        errno = 0;
        fprintf(stderr, "Transferência com saldo insuficiente!\n");
        return 0;
    }

    org->saldo = org->saldo - quant;
    dest->saldo = dest->saldo + quant;
    puts("Transferência realizada com sucesso.");
}

/// @brief Deleta usuário a partir do id; Esta função não remove a instância da lista, apenas esvazia ela
int delete_user(int id){
    struct cliente *c = find_user_ptr(id);
    if(c == NULL){ return 0; }
    printf("Cliente com id %d deletado.", id);
    *c = cliente_vazio;
    return 1;
}