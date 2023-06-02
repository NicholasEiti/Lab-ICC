#include "bank-utils.h"
#include <string.h>

unsigned int counter = 0;
unsigned int id = 0;

/// @brief Cria um usuário com id, nome, idade e saldo
int create_user(struct cliente lista[], char nome[100], unsigned int idade, float saldo){
    struct cliente novo_cliente;
    int teste;
    novo_cliente.id = ++id;
    strcpy(novo_cliente.nome, nome);
    novo_cliente.idade = idade;
    novo_cliente.saldo = saldo;
    lista[counter] = novo_cliente;
    counter++;
    return 1;
}

/// @brief Acha usuário a partir do id
/// @param lista a lista de clientes
/// @param id identificação do cliente
/// @return Retorna o index do usuário em referência a lista; retorna -1 caso o usuário não seja encontrado/inválido
int find_user(struct cliente lista[], int id){
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
int transfer(struct cliente lista[], int id_orig, int id_dest, float quant){
    // Achar o usuário id_orig
    int index_orig = find_user(lista, id_orig);
    if(index_orig == -1){ return 0; }

    // Achar o usuário id_dest
    int index_dest = find_user(lista, id_dest);
    if(index_dest == -1){ return 0; }

    if(lista[index_orig].saldo < quant){
        return -1;
    }

    lista[index_orig].saldo -= quant;
    lista[index_dest].saldo += quant;
}

/// @brief Deleta usuário a partir do id; Esta função não remove a instância da lista, apenas esvazia ela
int delete_user(struct cliente lista[], int id){
    int user_index = find_user(lista, id);
    if(user_index == -1){
        return 0;
    }

    // Não estamos deletando, estamos apenas deixando o usuário vazio
    strcpy(lista[user_index].nome, "");
    lista[user_index].idade = 0;
    lista[user_index].saldo = 0;
}