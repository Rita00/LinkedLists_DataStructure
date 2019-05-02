#include <stdio.h>
#include <strings.h>
#include "estruturas.h"


/*
void load_ficheiro_locais(lista_locais *BDlocais, lista_PDIs *BDpdis){
    FILE *ficheiro_p;
    ficheiro_p = fopen(NOME_FICHEIRO,'r');


}*/

int main() {
    local *aux;
    lista_locais *BDlocais;
    lista_PDIs *BDpdis;
    BDlocais = inicia_lista_locais();
    BDpdis = inicia_lista_PDIs();
    /*codigo de teste*/
    aux = cria_local("Coimbra", BDlocais);
    cria_local("A", BDlocais);
    cria_local("C", BDlocais);
    cria_PDI("pdi1", "jfdksnfkdan", "00:05-00:45", BDpdis, aux);
    cria_PDI("pdi5", "jfdksnfkdan", "00:05-00:45", BDpdis, aux);
    cria_PDI("pdi3", "jfdksnfkdan", "00:05-00:45", BDpdis, aux);
    cria_local("SDF", BDlocais);
    aux = cria_local("SFD", BDlocais);
    cria_PDI("pdi2", "jfdksnfkdan", "00:05-00:45", BDpdis, aux);
    cria_PDI("pdi7", "jfdksnfkdan", "00:05-00:45", BDpdis, aux);
    imprime_lista_locais(BDlocais);
    imprime_lista_PDIs(BDpdis);
    printf("-----------------");
    imprime_lista_PDIs(aux->pontos);
    printf("-----------------");
    /*carregar informacao do ficheiro para listas ligadas*/


    /*criar menus*/


    /*desalocacao de memoria*/
    destroi_lista_locais(BDlocais);
    destroi_lista_PDIs(BDpdis);
    return 0;
}
/*LISTAS LOCAIS
 *
 *
 *
 *
 * */

void elimina_lista_locais(lista_locais *lista){
    lista_locais *aux = lista;
    while(lista != NULL){
        aux = lista;
        lista = lista->next;
        free(aux);
    }
}

void elimina_local(local *loc){
    free(loc->nome_local);
    elimina_lista_pdis(loc->pontos);
    free(loc);
}

void destroi_lista_locais(lista_locais *BDlocais){
    lista_locais *aux=BDlocais;
    while(BDlocais != NULL){
        aux = BDlocais;
        BDlocais = BDlocais->next;
        elimina_local(aux->loc);
        free(aux);
    }
}

lista_locais* inicia_lista_locais(){
    lista_locais *aux = (lista_locais*) malloc(sizeof(lista_locais));
    aux->loc = 0;
    aux->next = NULL;
    return aux;
}

lista_locais* cria_lista_locais(local* loc, lista_locais *next){
    lista_locais *aux = (lista_locais*)malloc(sizeof(lista_locais));
    aux->loc = loc;
    aux->next = next;
    return aux;
}

local *cria_local(char *nome, lista_locais *BD_locais){ /*usar para carregar dados dos ficheiros*/
    local * aux;
    long id;
    id = (long) BD_locais->loc/sizeof(local);/*loc is pointer and is incremented every 24 bytes in a 64-bits machine*/
    BD_locais->loc++;
    aux = (local*)malloc(sizeof(local));
    aux->nome_local = strdup(nome); /*aloca memoria para string passada como argumento e devolve o seu ponteiro, guardando na estrutura local*/
    aux->id = id;
    aux->pontos = inicia_lista_PDIs();
    inserir_local(BD_locais, aux);
    return aux;
}

void inserir_local(lista_locais *lista, local *loc){
    lista_locais *aux_lista;
    /**/
    while (lista->next != NULL && strcmp(loc->nome_local, lista->next->loc->nome_local) > 0){
        lista = lista->next;
        /*desnecessario verificar se IDs iguais, porque gerados pelo programa*/
    }
    aux_lista = cria_lista_locais(loc, lista->next);
    lista->next = aux_lista;
}


void imprime_lista_locais(lista_locais *lista) {
    while (lista->next != NULL) {
        lista = lista->next;
        printf("%d\n%s\n\n", lista->loc->id, lista->loc->nome_local);
    }
}

/*LISTAS PDIS
 *
 *
 *
 *
 * */

void elimina_lista_pdis(lista_PDIs *lista){
    lista_PDIs *aux;
    while(lista != NULL){
        aux = lista;
        lista = lista->next;
        free(aux);
    }
}

void elimina_PDI(PDI *ponto){
    free(ponto->nome_PDI);
    free(ponto->descricao);
    free(ponto->horario);
    free(ponto);
}

void destroi_lista_PDIs(lista_PDIs *BDPDIs){
    lista_PDIs *aux;
    while(BDPDIs != NULL){
        aux = BDPDIs;
        BDPDIs = BDPDIs->next;
        elimina_PDI(aux->ponto_interesse);
        free(aux);
    }
}

lista_PDIs* inicia_lista_PDIs(){
    lista_PDIs *aux = (lista_PDIs*) malloc(sizeof(lista_PDIs));
    aux->ponto_interesse = 0;
    aux->next = NULL;
    return aux;
}

lista_PDIs* cria_lista_PDIs(PDI* ponto, lista_PDIs *next){
    lista_PDIs *aux = (lista_PDIs*)malloc(sizeof(lista_PDIs));
    aux->ponto_interesse = ponto;
    aux->next = next;
    return aux;
}

void cria_PDI(char *nome, char *descricao, char *horario, lista_PDIs *BD_PDIs, local *loc){ /*usar para carregar dados dos ficheiros*/
    PDI * aux;
    long id;
    id = (long) BD_PDIs->ponto_interesse/sizeof(PDI);
    BD_PDIs->ponto_interesse++;
    aux = (PDI*)malloc(sizeof(PDI));
    aux->nome_PDI = strdup(nome); /*aloca memoria para string passada como argumento e devolve o seu ponteiro, guardando na estrutura local*/
    aux->descricao = strdup(descricao);
    aux->horario = strdup(horario);
    aux->id = id;
    aux->loc = loc;
    inserir_PDI(BD_PDIs, aux);
    inserir_PDI(loc->pontos, aux);
}

void inserir_PDI(lista_PDIs *lista, PDI *ponto){
    lista_PDIs *aux_lista;
    /**/
    while (lista->next != NULL && strcmp(ponto->nome_PDI, lista->next->ponto_interesse->nome_PDI) > 0){
        lista = lista->next;
        /*desnecessario verificar se IDs iguais, porque gerados pelo programa*/
    }
    aux_lista = cria_lista_PDIs(ponto, lista->next);
    lista->next = aux_lista;
}


void imprime_lista_PDIs(lista_PDIs *lista) {
    while (lista->next != NULL) {
        lista = lista->next;
        printf("%d\n%s\n%s\n%s\n\n", lista->ponto_interesse->id, lista->ponto_interesse->nome_PDI, lista->ponto_interesse->descricao, lista->ponto_interesse->horario);
    }
}

