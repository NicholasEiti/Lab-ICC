#include "bank-utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

unsigned int counter = 0;
unsigned int id = 0;

struct cliente *lista_clientes;

const struct cliente cliente_vazio = { -1, "", 0, 0 };

void initialize_list(){
    lista_clientes = malloc(0);
}

void print_user_data(struct cliente c){
    printf("ID: %d | Nome: %s | Idade: %u | Saldo: R$%.2f\n", c.id, c.nome, c.idade, c.saldo);
}

void list_all_users(){
    for (int i = 0; i < counter; i++){
        if(lista_clientes[i].id != -1){
            print_user_data(lista_clientes[i]);
        }
    }
}

void report(){
    FILE *report;
    char filename[50];
    time_t curtime = time(NULL);
    struct tm *tm_struct = localtime(&curtime);
    sprintf(filename, "%d-%d-%d-report.csv\n", tm_struct->tm_year + 1900, tm_struct->tm_mon+1, tm_struct->tm_mday);
    report = fopen(filename, "w+");
    fprintf(report, "ID,Nome,Idade,Saldo\n");
    for (int i = 0; i < counter; i++){
        struct cliente c = lista_clientes[i];
        fprintf(report, "%d,%s,%u,%.2f\n", c.id, c.nome, c.idade, c.saldo);
    }
    free(lista_clientes);
    fclose(report);
}

/// @brief Cria um usuário na lista
int create_user(struct cliente novo_cliente){
    // lista_clientes_dif[counter] = novo_cliente;
    // lista_clientes_dif[counter].id = ++id;
    lista_clientes = (struct cliente*)realloc(lista_clientes, (++counter)*sizeof(struct cliente));
    lista_clientes[counter-1] = novo_cliente;
    lista_clientes[counter-1].id = ++id;
    printf("Cliente inserido com id %d com sucesso\n", id);
    return 1;
}

int create_users(struct cliente novos_clientes[], int size){
    if(size < 1){ return 0; }
    printf("Clientes inseridos com id ");
    lista_clientes = (struct cliente*)realloc(lista_clientes, (counter + size)*sizeof(struct cliente));
    for(int i = 0; i < size; i++){
        counter++;
        lista_clientes[counter-1] = novos_clientes[i];
        lista_clientes[counter-1].id = ++id;
        printf("%d", id);
        if(i != size-1){
            printf(",");
        }
    }
    printf(" com sucesso\n");
}

/// @brief Acha usuário a partir do id
/// @param lista a lista de clientes
/// @param id identificação do cliente
/// @return Retorna o usuário de acordo com id; Retorna um usuário vazio caso o usuário não seja encontrado
struct cliente find_user(int id){
    for(int i = 0; i < counter; i++){
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
    for(int i = 0; i < counter; i++){
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
    if(org == NULL){ return 0; }

    // Achar o usuário id_dest
    struct cliente *dest = find_user_ptr(id_dest);
    if(dest == NULL){ return 0; }

    if(org == dest){
        errno = 0;
        fprintf(stderr, "Transferencia entre mesmo usuario!\n");
        return 0;
    }
    else if(quant <= 0){
        errno = 0;
        fprintf(stderr, "Saldo invalido!\n");
        return 0;
    }
    else if(org->saldo < quant){
        errno = 0;
        fprintf(stderr, "Transferencia com saldo insuficiente!\n");
        return 0;
    }
    
    org->saldo -= quant;
    dest->saldo += quant;
    return 1;
}

int delete_user(int id){
    struct cliente *c = find_user_ptr(id);
    if(c == NULL){ return 0; }
    for(int index = c - lista_clientes; index < counter; index++){
        lista_clientes[index] = lista_clientes[index+1];
    }
    lista_clientes = realloc(lista_clientes, (--counter)*sizeof(struct cliente));
    printf("Cliente com id %d deletado.", id);
    return 1;
}
