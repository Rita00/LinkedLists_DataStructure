#ifndef PROJETO2_0_ESTRUTURAS_H
#define PROJETO2_0_ESTRUTURAS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NOME_F_LOCAIS "Ficheiro_projeto.txt"
#define BUFFER_SIZE 1000
#define INT_DIM 11
#define ANO_ATUAL 2019
#define ANO_INICIAL 1900
#define N_TEL_MIN 910000000
#define N_TEL_MAX 969999999
#define N_TEL_MED_MIN 940000000
#define N_TEL_MED_MAX 960000000
#define NOME_F_UTILIZADOR "Ficheiro_utilizador"

typedef struct _PDI PDI;
typedef struct _local local;
typedef struct _utilizador utilizador;

typedef struct _lista_locais lista_locais;
typedef struct _lista_PDIs lista_PDIs;
typedef struct _lista_utilizadores lista_utilizadores;

typedef struct{ /*estrutura para data de nascimento*/
    int dia,mes,ano;
}data;

struct _utilizador{ /*estrutura para registo de utilizador*/
    int id; /*identificador único*/
    char *nome_utilizador;
    char *morada;
    data nascimento;
    int telefone;
    lista_locais *locais_escolhidos;
    lista_PDIs *pdis_preferidos;
    PDI *hot;
};

struct _PDI{ /*estrutura para pontos de interesse*/
    int id; /*identificador único*/
    char *nome_PDI;
    char *descricao;
    char *horario;
    lista_utilizadores *util; /*lista de utilizadores que preferiram este PDI*/
    lista_utilizadores *hot;
    local *loc;
};

struct _local{ /*estrutura de cada local e respetivos pontos de interesse*/
    int id; /*identificador único*/
    char *nome_local;
    lista_PDIs *pontos;//local tem lista ligada de PDIs
    lista_PDIs *pontospop;
    lista_utilizadores *util; /*lista de utilizadores que preferiram este local*/
};

struct _lista_utilizadores{ /*estrutura para lista de utilizadores*/
    utilizador *pessoa;
    lista_utilizadores *next;
};

struct _lista_PDIs{ /*estrutura para lista de pontos de interesse*/
    PDI *ponto_interesse;
    lista_PDIs *next;
};

struct _lista_locais{ /*estrutura para lista de locais*/
    local *loc; /*aponta para uma estrutura local*/
    lista_locais *next; /*aponta para o elemento seguinte da lista*/
};

/*LISTAS LOCAIS*/
local *pesquisa_local(lista_locais *lista, int id_a_encontrar);
void elimina_lista_locais(lista_locais *BDlocais);
void destroi_lista_locais(lista_locais *BDlocais);
void elimina_local(local *loc);
lista_locais* inicia_lista_locais();
lista_locais* cria_lista_locais(local* loc, lista_locais *next);
local *cria_local(char *nome, lista_locais *BD_locais);
void inserir_local(lista_locais *lista, local *loc);
void imprime_lista_locais(lista_locais *lista);
void imprime_local_pdis_alfabetica(lista_locais *BDlocais);
char remove_local(lista_locais *BDlocais, int id);


/*LISTAS PDIS*/
PDI *pesquisa_PDI(lista_PDIs *lista, int id_a_encontrar);
void destroi_lista_PDIs(lista_PDIs *BDPDIs);
void elimina_PDI(PDI *ponto);
void elimina_lista_pdis(lista_PDIs *lista);
lista_PDIs* inicia_lista_PDIs();
lista_PDIs* cria_lista_PDIs(PDI* ponto, lista_PDIs *next);
PDI *cria_PDI(char *nome, char *descricao, char *horario, lista_PDIs *BD_PDIs, local *PDI);
void inserir_PDI(lista_PDIs *lista, PDI *loc);
void imprime_lista_PDIs(lista_PDIs *lista);
char remove_PDI(lista_PDIs *BDpdis, int id);

/*LISTAS UTILIZADORES*/
utilizador *pesquisa_utilizador(lista_utilizadores *lista, int id_a_encontrar);
void elimina_lista_utlizadores(lista_utilizadores *lista);
void elimina_utilizador(utilizador *pessoa);
char remove_utilizador(lista_utilizadores *BDutilizadores, int id);
void destroi_lista_utilizadores(lista_utilizadores *BDutilizadores);
lista_utilizadores *inicia_lista_utilizadores();
lista_utilizadores *cria_lista_utilizadores(utilizador *humano, lista_utilizadores *next);
utilizador* cria_utilizador(char *nome, char *address, int dia, int mes, int ano, int telefone, lista_utilizadores *BD_utilizadores);
void inserir_utilizador(lista_utilizadores *lista, utilizador *humano);
void imprime_lista_utilizadores(lista_utilizadores *lista);

/*menus*/
void menu_registo(lista_utilizadores *BDutilizadores);
void menu_escolha_locais(lista_locais *BDlocais, lista_utilizadores *BDutilizadores, lista_locais *BDlocaisPop);
void menu_escolha_pdis(lista_PDIs *BDPDIs, lista_utilizadores *BDutilizadores);
void menu_listagens(lista_locais *BDlocais, lista_locais *BDlocaisPop);
void menu_escolha_pdi_hot(lista_PDIs *BDPDIs, lista_utilizadores *BDutilizadores);
void menu_viagem(lista_utilizadores *BDutilizadores, lista_PDIs *BDPDIs);

/*funcoes auxiliares*/
void remove_nova_linha(char *frase);
int conta_utilizadores(lista_utilizadores *lista);
int conta_locais(lista_locais *lista);
int conta_PDIs(lista_PDIs *lista);
lista_PDIs *constroi_viagem(int id_util, lista_utilizadores *BDutilizadores);
float percentagem_local_preferido(lista_locais *locais_util, lista_utilizadores *BDutilizadores);
float percentagem_PDI_hot(lista_utilizadores *BDutilizadores, lista_PDIs *viagem);
float percentagem_preferencias_PDI(lista_PDIs *BDPDIs, lista_PDIs *viagem);

/*protecao dados*/
void clear_input();
char input_number(int *number);
void input_errors(char erro);
void get_number(int *num);
char verifica_data(int dia, int mes, int ano);
void data_erros(char erro);
char verifica_telefone(int telefone);


/*ficheiros*/
void load_ficheiro_locais(lista_locais *BDlocais, lista_PDIs *BDpdis, lista_locais *BDlocaisPop);
void write_ficheiro_utilizador(lista_utilizadores *BDutilizadores);
void load_ficheiro_utilizador(lista_utilizadores *BDutilizadores, lista_PDIs *BDPDIs, lista_locais *BDlocais, lista_locais *BDlocaisPop);




void ordena_localpop(int id_local, lista_locais *BDlocaisPop);
void ordena_PDIpop(int id_pdi, lista_PDIs *lista);
#endif //PROJETO2_0_ESTRUTURAS_H
