#include "bank-utils.h"
#include <stdio.h>
#include <string.h>

unsigned int counter = 0;
unsigned int id = 0;

struct cliente lista_clientes[100];

void initialize_list(){
    for(int i = 0; i < 100; i++){
        strcpy(lista_clientes[i].nome, "");
        lista_clientes[i].id = -1;
        lista_clientes[i].idade = 0;
        lista_clientes[i].saldo = 0;
    }
}

void list_all_users(){
    for (int i = 0; i < 100; i++){
        if(lista_clientes[i].id != -1){
            printf("id: %d nome: %s idade: %u saldo: R$%.2f\n", 
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
int find_user(int id){
    // for(int i = 0; i < 100; i++){
    //     if(lista[i].id == id && lista[i].nome != ""){
    //         return i;
    //     }
    // }
    // return -1;
}


/// @brief Transferência entre dois usuários
/// @param id_orig id da origem da transferência
/// @param id_dest id do alvo da transferência
/// @param quant valor da transferência
/// @return Retorna 1 caso a transferência tenha sucesso; retorna 0 caso um dos usuários não seja encontrado; 
/// retorna -1 caso a transferência seja inválida por outro motivo
int transfer(int id_orig, int id_dest, float quant){
    // Achar o usuário id_orig
    int index_orig = find_user(id_orig);
    if(index_orig == -1){ return 0; }

    // Achar o usuário id_dest
    int index_dest = find_user(id_dest);
    if(index_dest == -1){ return 0; }

    if(lista_clientes[index_orig].saldo < quant){
        return -1;
    }

    lista_clientes[index_orig].saldo -= quant;
    lista_clientes[index_dest].saldo += quant;
}

/// @brief Deleta usuário a partir do id; Esta função não remove a instância da lista, apenas esvazia ela
int delete_user(int id){
    int user_index = find_user(id);
    if(user_index == -1){
        return 0;
    }

    // Não estamos deletando, estamos apenas deixando o usuário vazio
    strcpy(lista_clientes[user_index].nome, "");
    lista_clientes[user_index].id = -1;
    lista_clientes[user_index].idade = 0;
    lista_clientes[user_index].saldo = 0;
}