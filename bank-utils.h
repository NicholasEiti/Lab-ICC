#ifndef BANK_UTILS_
#define BANK_UTILS_

#define NOME_LEN 100

struct cliente{
    int id;
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

// Cria relatório da lista de clientes cadastrados
void report();

// Adiciona um cliente na lista de clientes
int create_user(struct cliente novo_cliente);

// Adiciona vários clientes na lista de clientes
int create_users(struct cliente novos_clientes[], int size);

// Retorna o usuário com o id do argumento; retorna cliente_vazio caso o cliente não seja encontrado
struct cliente find_user(int id);

// Realiza a transferência entre dois usuários
int transfer(int id_orig, int id_dest, float quant);

// Deleta um usuário com o id do argumento
int delete_user(int id);

// Processa linha dos dados de criaçao de cliente
int client_input(struct cliente *new_cliente);

// Processa linha dos dados de transferência
int transfer_input(int *id_orig, int *id_dest, float *quant);

#endif
