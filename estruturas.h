//
// Created by rilap on 01/05/2019.
//

#ifndef PROJETO2_0_ESTRUTURAS_H
#define PROJETO2_0_ESTRUTURAS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NOME_FICHEIRO "Ficheiro_projeto.txt"
#define BUFFER_SIZE 1000

typedef struct _PDI PDI;
typedef struct _local local;
typedef struct _utilizador utilizador;

typedef struct _lista_locais lista_locais;
typedef struct _lista_PDIs lista_PDIs;
typedef struct _lista_utilizadores lista_utilizadores;

typedef struct{ /*estrutura para data de nascimento*/
    int dia,mes,ano;
}data;

typedef struct _utilizador{ /*estrutura para registo de utilizador*/
    int id; /*identificador único*/
    char *nome;
    char *morada;
    data nascimento;
    int telefone;
    lista_locais *locais_escolhidos;
    lista_PDIs *pdis_preferidos;
    PDI *hot;
}utilizador;

typedef struct _PDI{ /*estrutura para pontos de interesse*/
    int id; /*identificador único*/
    char *nome_PDI;
    char *descricao;
    char *horario;
    int popularidade;
    local *loc;
}PDI;

typedef struct _local{ /*estrutura de cada local e respetivos pontos de interesse*/
    int id; /*identificador único*/
    char *nome_local;
    lista_PDIs *pontos;//local tem lista ligada de PDIs
}local;

typedef struct _lista_utilizadores{ /*estrutura para lista de utilizadores*/
    utilizador *pessoa;
    lista_utilizadores *next;
}lista_utilizadores;

typedef struct _lista_PDIs{ /*estrutura para lista de pontos de interesse*/
    PDI *ponto_interesse;
    lista_PDIs *next;
}lista_PDIs;

typedef struct _lista_locais{ /*estrutura para lista de locais*/
    local *loc; /*aponta para uma estrutura local*/
    lista_locais *next; /*aponta para o elemento seguinte da lista*/
}lista_locais;

/*LISTAS LOCAIS*/
void elimina_lista_locais(lista_locais *BDlocais);
void destroi_lista_locais(lista_locais *BDlocais);
void elimina_local(local *loc);
lista_locais* inicia_lista_locais();
lista_locais* cria_lista_locais(local* loc, lista_locais *next);
local *cria_local(char *nome, lista_locais *BD_locais);
void inserir_local(lista_locais *lista, local *loc);
void imprime_lista_locais(lista_locais *lista);


/*LISTAS PDIS*/
void destroi_lista_PDIs(lista_PDIs *BDPDIs);
void elimina_PDI(PDI *ponto);
void elimina_lista_pdis(lista_PDIs *lista);
lista_PDIs* inicia_lista_PDIs();
lista_PDIs* cria_lista_PDIs(PDI* ponto, lista_PDIs *next);
void cria_PDI(char *nome, char *descricao, char *horario, lista_PDIs *BD_PDIs, local *PDI);
void inserir_PDI(lista_PDIs *lista, PDI *loc);
void imprime_lista_PDIs(lista_PDIs *lista);








#endif //PROJETO2_0_ESTRUTURAS_H
