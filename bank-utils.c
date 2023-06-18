#include "bank-utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <uuid/uuid.h>

unsigned int counter = 0;
unsigned int id = 0;
char sha1namespace[UUID_STR_LEN];

struct cliente *lista_clientes;

struct cliente cliente_vazio = { "\0", "", 0, 0 };

void initialize_list(){
    lista_clientes = malloc(0);
    uuid_t namespace_uuid;
    uuid_generate_random(namespace_uuid);
    uuid_unparse(namespace_uuid, sha1namespace);
}

void print_user_data(struct cliente c){
    char uuid_text[UUID_STR_LEN];
    uuid_unparse(c.id, uuid_text);
    printf("ID: %s | Nome: %s | Idade: %u | Saldo: R$%.2f\n", uuid_text, c.nome, c.idade, c.saldo);
}

void list_all_users(){
    for (int i = 0; i < counter; i++){
        if(!uuid_is_null(lista_clientes[i].id)){
            print_user_data(lista_clientes[i]);
        }
    }
}

void create_uuid(uuid_t uuid, char plain[6]){
    uuid_t uuid_namespace;
    uuid_parse(sha1namespace, uuid_namespace);
    uuid_generate_sha1(uuid, uuid_namespace, plain, 6);
}

void report(){
    FILE *report;
    char filename[50];

    // Criando o nome do arquivo (de acordo com tempo)
    time_t curtime = time(NULL);
    struct tm *tm_struct = localtime(&curtime);
    sprintf(filename, "%d-%.2d-%.2d %.2d-%.2d-%.2d report.csv", tm_struct->tm_year + 1900, tm_struct->tm_mon+1, tm_struct->tm_mday, 
    tm_struct->tm_hour, tm_struct->tm_min, tm_struct->tm_sec);
    report = fopen(filename, "w+");

    // Cabeçalho do arquivo
    fprintf(report, "ID,Nome,Idade,Saldo\n"); 
    for (int i = 0; i < counter; i++){
        struct cliente c = lista_clientes[i];
        char uuid_text[UUID_STR_LEN];
        uuid_unparse(c.id, uuid_text);
        fprintf(report, "%s,%s,%u,%.2f\n", uuid_text, c.nome, c.idade, c.saldo);
    }
    fprintf(report, "$end,%s,%u,%u", sha1namespace, id, counter);

    free(lista_clientes);
    fclose(report);
}

int load_report(char* filename){
    FILE* r_report;
    r_report = fopen(filename, "r+");

    if(r_report == NULL){
        fprintf(stderr, "Arquivo inexistente!\n");
        return 0;
    }

    char *output;
    size_t buffer_size = 0;
    getline(&output, &buffer_size, r_report);

    if(strcmp(output, "ID,Nome,Idade,Saldo\n")){
        fprintf(stderr, "Arquivo nao compativel\n");
        return 0;
    }
    
    
    while(!feof(r_report)){
        char** usr_in;
        usr_in = malloc(NOME_LEN+40);
        if(!(line_input(r_report, 4, NOME_LEN+40, ",", usr_in))){
            fprintf(stderr, "Erro: linha em formato incompatível\n");
            return 0;
        }

        if(strcmp(usr_in[0], "$end") == 0){
            strcpy(sha1namespace, usr_in[1]);

            char *endptr;
            unsigned int temp = strtoul(usr_in[2], &endptr, 10);
            if(*endptr != '\0'){
                fprintf(stderr, "Erro: id inicial invalido\n");
                return 0;
            }
            id = temp;

            temp = strtoul(usr_in[3], &endptr, 10);
            if(*endptr != '\0'){
                fprintf(stderr, "Erro: counter inicial invalido\n");
                return 0;
            }
            counter = temp;
            
            return 1;
        }

        struct cliente new_cliente;

        uuid_t new_uuid;
        uuid_parse(usr_in[0], new_uuid);

        if(client_data(usr_in[1], usr_in[2], usr_in[3], &new_cliente)){
            uuid_copy(new_cliente.id, new_uuid);
            lista_clientes = (struct cliente*)realloc(lista_clientes, (++counter)*sizeof(struct cliente));
            lista_clientes[counter-1] = new_cliente;

            puts("Cliente inserido com sucesso");
        }
        else{
            fprintf(stderr, "Erro na leitura da linha\n");
            return 0;
        }
    }
    fprintf(stderr, "Final do arquivo invalido\n");
    return 0;
}

int create_user(struct cliente novo_cliente){
    lista_clientes = (struct cliente*)realloc(lista_clientes, (++counter)*sizeof(struct cliente));
    lista_clientes[counter-1] = novo_cliente;
    id++;

    char idtxt[6];
    sprintf(idtxt, "%.6u", id);

    uuid_t newuuid;
    create_uuid(newuuid, idtxt);
    uuid_copy(lista_clientes[counter-1].id, newuuid);
    printf("Cliente inserido com id %s com sucesso\n", idtxt);
    return 1;
}

int create_users(struct cliente novos_clientes[], int size){
    if(size < 1){ return 0; }
    printf("Clientes inseridos com id ");
    lista_clientes = (struct cliente*)realloc(lista_clientes, (counter + size)*sizeof(struct cliente));

    for(int i = 0; i < size; i++){
        counter++;
        lista_clientes[counter-1] = novos_clientes[i];
        id++;

        char idtxt[7];
        sprintf(idtxt, "%.6u", id);
        uuid_t newuuid;
        create_uuid(newuuid, idtxt);
        uuid_copy(lista_clientes[counter-1].id, newuuid);

        printf("%s", idtxt);
        if(i != size-1){
            printf(", ");
        }
    }
    printf(" com sucesso\n");
    return 1;
}


struct cliente find_user(unsigned int trg_id){
    char idtxt[6];
    sprintf(idtxt, "%.6u", trg_id);

    uuid_t uuid;
    create_uuid(uuid, idtxt);
    char* teste;
    // uuid_unparse(uuid, teste);
    // printf("%s\n", teste);
    // printf("%u\n", counter);
    for(int i = 0; i < counter; i++){
        if(!uuid_compare(uuid, lista_clientes[i].id)){
            return lista_clientes[i];
        }
    }
    errno = 0;
    fprintf(stderr, "Cliente com id %s inesistente!\n", idtxt);
    return cliente_vazio;
}

struct cliente *find_user_ptr(unsigned int trg_id){
    char idtxt[6];
    sprintf(idtxt, "%.6u", trg_id);

    uuid_t uuid;
    create_uuid(uuid, idtxt);

    for(int i = 0; i < counter; i++){
        if(!uuid_compare(uuid, lista_clientes[i].id)){
            return &lista_clientes[i];
        }
    }
    errno = 0;
    fprintf(stderr, "Cliente com id %s inesistente!\n", idtxt);
    return NULL;
}

int transfer(unsigned int id_orig, unsigned int id_dest, float quant){
    // Achar o usuário id_orig
    struct cliente *org = find_user_ptr(id_orig);
    if(org == NULL){ return 0; }

    // Achar o usuário id_dest
    struct cliente *dest = find_user_ptr(id_dest);
    if(dest == NULL){ return 0; }

    // Verificação de erros
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
    
    // Transferência
    org->saldo -= quant;
    dest->saldo += quant;
    return 1;
}

int delete_user(unsigned int trg_id){
    struct cliente *cp = find_user_ptr(trg_id);
    if(cp == NULL){ return 0; }
    
    // Deslocar os itens não deletados
    for(int index = cp - lista_clientes; index < counter-1; index++){
        lista_clientes[index] = lista_clientes[index+1];
    }
    lista_clientes = realloc(lista_clientes, (--counter)*sizeof(struct cliente));
    printf("Cliente com id %d deletado.", trg_id);
    return 1;
}
