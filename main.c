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
    PDI *aux2;
    utilizador *aux3;
    int opcao = 1;
    lista_locais *BDlocais;
    lista_PDIs *BDpdis;
    lista_utilizadores *BDutilizadores;
    BDlocais = inicia_lista_locais();
    BDpdis = inicia_lista_PDIs();
    BDutilizadores = inicia_lista_utilizadores();
    /*codigo de teste*/
    aux = cria_local("Coimbra", BDlocais);
    cria_local("A", BDlocais);
    cria_local("C", BDlocais);
    cria_PDI("pdi1", "jfdksnfkdan", "00:05-00:45", BDpdis, aux);
    cria_PDI("pdi5", "jfdksnfkdan", "00:05-00:45", BDpdis, aux);
    cria_PDI("pdi3", "jfdksnfkdan", "00:05-00:45", BDpdis, aux);
    cria_local("SDF", BDlocais);
    aux = cria_local("SFD", BDlocais);
    aux2 = cria_PDI("pdi2", "jfdksnfkdan", "00:05-00:45", BDpdis, aux);
    cria_PDI("pdi7", "jfdksnfkdan", "00:05-00:45", BDpdis, aux);
    printf("%d %s\n", pesquisa_local(BDlocais, 2)->id, pesquisa_local(BDlocais, 2)->nome_local);
    printf("%d %s\n", pesquisa_PDI(BDpdis, 3)->id, pesquisa_PDI(BDpdis, 3)->nome_PDI);

    cria_utilizador("Rita", "ADAF", 2, 4, 1990, 96776790, BDutilizadores);
    cria_utilizador("Francisco", "afaefa", 25, 11, 1997, 962025039, BDutilizadores);
    cria_utilizador("Ana", "dweqf", 25, 4, 2000, 963245234, BDutilizadores);
    cria_utilizador("bjkfw", "f3", 3, 6, 1995, 964561284, BDutilizadores);
    /*cria_utilizador faz a alocação de memoria para um utilizador E DE SEGUIDA CHAMA inserir_utilizador para inserir na base de dados. devolve também o ponteiro para o utilizador criado*/
    /*inserir_utilizador recebe um ponteiro para um utilizador e um ponteiro para uma lista_ligada de utilizadores e insere o utilizador nessa lista. NÃO CRIA um utilizador*/
    /*aqui crias um utilizador(o que faz com que seja automaticamente adiciona à base de dados, e de seguida inseres na base de dados, daí a duplicação*/
    /*inserir_utilizador(BDutilizadores,cria_utilizador("helia","dkjbcfa",10,9,1971,965347894,BDutilizadores));*/
    remove_utilizador(BDutilizadores, 2);
    cria_utilizador("dfaf", "faa", 3, 7, 2001, 465327483, BDutilizadores);
    cria_utilizador("adfa", "faf", 65, 3, 3342, 423525, BDutilizadores);
    imprime_lista_utilizadores(BDutilizadores);
    /*carregar informacao do ficheiro para listas ligadas*/

    /*criar menus*/
    while (opcao != 0) {
        printf("1- Registo\n");
        printf("2- Listagem de locais\n");
        printf("3- Listagem de utilizadores\n");
        printf("4- Escolha locais\n");
        printf("5- Escolha pdis\n");
        printf("6- Escolha pdi hot\n");
        printf("7- Construir viagem\n");
        printf("0- Sair\n");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                menu_registo(BDutilizadores);
                break;
            case 2:
                menu_listagens(BDlocais);
                break;
            case 3:
                imprime_lista_utilizadores(BDutilizadores);
                break;
            case 4:
                menu_escolha_locais(BDlocais, BDutilizadores);
                break;
            case 5:
                menu_escolha_pdis(BDpdis, BDutilizadores);
                break;
            case 6:
                menu_escolha_pdi_hot(BDpdis, BDutilizadores);
                break;
            case 7:
                break;
            default:
                break;
        }
    }
    imprime_lista_utilizadores(BDutilizadores);
    /*desalocacao de memoria*/
    destroi_lista_locais(BDlocais);
    destroi_lista_PDIs(BDpdis);
    destroi_lista_utilizadores(BDutilizadores);
    return 0;
}

/*LISTAS LOCAIS
 *
 *
 *
 *
 * */
local *pesquisa_local(lista_locais *lista,
                      int id_a_encontrar) { /*devolve o nó da lista se for encontrado, ou NULL se inexistente*/
    lista = lista->next;
    while (lista != NULL && lista->loc->id != id_a_encontrar) {
        lista = lista->next;
    }
    return lista->loc;
}

void elimina_lista_locais(lista_locais *lista) {
    lista_locais *aux = lista;
    while (lista != NULL) {
        aux = lista;
        lista = lista->next;
        free(aux);
    }
}

void elimina_local(local *loc) {
    free(loc->nome_local);
    elimina_lista_pdis(loc->pontos);
    free(loc);
}

void destroi_lista_locais(lista_locais *BDlocais) {
    lista_locais *aux = BDlocais;
    while (BDlocais != NULL) {
        aux = BDlocais;
        BDlocais = BDlocais->next;
        elimina_local(aux->loc);
        free(aux);
    }
}

lista_locais *inicia_lista_locais() {
    lista_locais *aux = (lista_locais *) malloc(sizeof(lista_locais));
    aux->loc = 0;
    aux->next = NULL;
    return aux;
}

lista_locais *cria_lista_locais(local *loc, lista_locais *next) {
    lista_locais *aux = (lista_locais *) malloc(sizeof(lista_locais));
    aux->loc = loc;
    aux->next = next;
    return aux;
}

local *cria_local(char *nome, lista_locais *BD_locais) { /*usar para carregar dados dos ficheiros*/
    local *aux;
    long id;
    id = (long) BD_locais->loc / sizeof(local);/*loc is pointer and is incremented every 24 bytes in a 64-bits machine*/
    BD_locais->loc++;
    aux = (local *) malloc(sizeof(local));
    aux->nome_local = strdup(
            nome); /*aloca memoria para string passada como argumento e devolve o seu ponteiro, guardando na estrutura local*/
    aux->id = id;
    aux->pontos = inicia_lista_PDIs();
    aux->util = inicia_lista_utilizadores();
    inserir_local(BD_locais, aux);
    return aux;
}

void inserir_local(lista_locais *lista, local *loc) {
    lista_locais *aux_lista;
    /**/
    while (lista->next != NULL && strcasecmp(loc->nome_local, lista->next->loc->nome_local) > 0) {
        lista = lista->next;
        /*desnecessario verificar se IDs iguais, porque gerados pelo programa*/
    }
    aux_lista = cria_lista_locais(loc, lista->next);
    lista->next = aux_lista;
}

void remove_local(lista_locais *BDlocais, int id) {
    lista_locais *aux;
    while (BDlocais->next != NULL && BDlocais->next->loc->id != id) {
        BDlocais = BDlocais->next;
    }
    if (BDlocais->next != NULL) {
        elimina_local(BDlocais->next->loc);
    }
    aux = BDlocais->next->next;
    free(BDlocais->next);
    BDlocais->next = aux;
}


void imprime_lista_locais(lista_locais *lista) {
    while (lista->next != NULL) {
        lista = lista->next;
        printf("%d\n%s\n\n", lista->loc->id, lista->loc->nome_local);
    }
}

void imprime_local_pdis(lista_locais *BDlocais) {
    lista_PDIs *aux;
    while (BDlocais->next != NULL) {
        printf("%d\n%s\n", BDlocais->next->loc->id, BDlocais->next->loc->nome_local);
        aux = BDlocais->next->loc->pontos;
        while (aux->next != NULL) {
            printf("\t%d\n\t%s\n\t%s\n\t%s\n", aux->next->ponto_interesse->id, aux->next->ponto_interesse->nome_PDI,
                   aux->next->ponto_interesse->descricao, aux->next->ponto_interesse->horario);
            aux = aux->next;

        }
        printf("\n");
        BDlocais = BDlocais->next;
    }
}

/*LISTAS PDIS
 *
 *
 *
 *
 * */
PDI *pesquisa_PDI(lista_PDIs *lista,
                  int id_a_encontrar) { /*devolve o nó da lista se for encontrado, ou NULL se inexistente*/
    lista = lista->next;
    while (lista != NULL && lista->ponto_interesse->id != id_a_encontrar) {
        lista = lista->next;
    }
    return lista->ponto_interesse;
}

void elimina_lista_pdis(lista_PDIs *lista) {
    lista_PDIs *aux;
    while (lista != NULL) {
        aux = lista;
        lista = lista->next;
        free(aux);
    }
}

void elimina_PDI(PDI *ponto) {
    free(ponto->nome_PDI);
    free(ponto->descricao);
    free(ponto->horario);
    free(ponto);
}

void destroi_lista_PDIs(lista_PDIs *BDPDIs) {
    lista_PDIs *aux;
    while (BDPDIs != NULL) {
        aux = BDPDIs;
        BDPDIs = BDPDIs->next;
        elimina_PDI(aux->ponto_interesse);
        free(aux);
    }
}

lista_PDIs *inicia_lista_PDIs() {
    lista_PDIs *aux = (lista_PDIs *) malloc(sizeof(lista_PDIs));
    aux->ponto_interesse = 0;
    aux->next = NULL;
    return aux;
}

lista_PDIs *cria_lista_PDIs(PDI *ponto, lista_PDIs *next) {
    lista_PDIs *aux = (lista_PDIs *) malloc(sizeof(lista_PDIs));
    aux->ponto_interesse = ponto;
    aux->next = next;
    return aux;
}

PDI *cria_PDI(char *nome, char *descricao, char *horario, lista_PDIs *BD_PDIs,
              local *loc) { /*usar para carregar dados dos ficheiros*/
    PDI *aux;
    long id;
    id = (long) BD_PDIs->ponto_interesse / sizeof(PDI);
    BD_PDIs->ponto_interesse++;
    aux = (PDI *) malloc(sizeof(PDI));
    aux->nome_PDI = strdup(
            nome); /*aloca memoria para string passada como argumento e devolve o seu ponteiro, guardando na estrutura local*/
    aux->descricao = strdup(descricao);
    aux->horario = strdup(horario);
    aux->id = id;
    aux->loc = loc;
    aux->util = inicia_lista_utilizadores();
    aux->hot = inicia_lista_utilizadores();
    inserir_PDI(BD_PDIs, aux);
    inserir_PDI(loc->pontos, aux);
    return aux;
}

void inserir_PDI(lista_PDIs *lista, PDI *ponto) {
    lista_PDIs *aux_lista;
    /**/
    while (lista->next != NULL && strcasecmp(ponto->nome_PDI, lista->next->ponto_interesse->nome_PDI) > 0) {
        lista = lista->next;
        /*desnecessario verificar se IDs iguais, porque gerados pelo programa*/
    }
    aux_lista = cria_lista_PDIs(ponto, lista->next);
    lista->next = aux_lista;
}


void imprime_lista_PDIs(lista_PDIs *lista) {
    while (lista->next != NULL) {
        lista = lista->next;
        printf("%d\n%s\n%s\n%s\n\n", lista->ponto_interesse->id, lista->ponto_interesse->nome_PDI,
               lista->ponto_interesse->descricao, lista->ponto_interesse->horario);
    }
}

void imprime_PDI_hot(int id_util, lista_utilizadores *BDutilizadores) {
    utilizador *util;
    util = pesquisa_utilizador(BDutilizadores, id_util);
    printf("%d\n%s\n%s\n%s\n\n", util->hot->id, util->hot->nome_PDI, util->hot->descricao, util->hot->horario);
}

void remove_PDI(lista_PDIs *BDpdis, int id) {
    lista_PDIs *aux;
    while (BDpdis->next != NULL && BDpdis->next->ponto_interesse->id != id) {
        BDpdis = BDpdis->next;
    }
    if (BDpdis->next != NULL) {
        elimina_PDI(BDpdis->next->ponto_interesse);
    }
    aux = BDpdis->next->next;
    free(BDpdis->next);
    BDpdis->next = aux;
}

/*lista de utilizadores
 *
 *
 *
 * */

utilizador *pesquisa_utilizador(lista_utilizadores *lista,
                                int id_a_encontrar) { /*devolve o nó da lista se for encontrado, ou NULL se inexistente*/
    lista = lista->next;
    while (lista != NULL && lista->pessoa->id != id_a_encontrar) {
        lista = lista->next;
    }
    return lista->pessoa;
}

void elimina_lista_utlizadores(lista_utilizadores *lista) {
    lista_utilizadores *aux = lista;
    while (lista != NULL) {
        aux = lista;
        lista = lista->next;
        free(aux);
    }
}

void elimina_utilizador(utilizador *pessoa) {
    free(pessoa->nome_utilizador);
    free(pessoa->morada);
    elimina_lista_locais(pessoa->locais_escolhidos);
    elimina_lista_pdis(pessoa->pdis_preferidos);
    free(pessoa);
}

void remove_utilizador(lista_utilizadores *BDutilizadores, int id) {
    lista_utilizadores *aux;
    while (BDutilizadores->next != NULL && BDutilizadores->next->pessoa->id != id) {
        BDutilizadores = BDutilizadores->next;
    }
    if (BDutilizadores->next != NULL) {
        elimina_utilizador(BDutilizadores->next->pessoa);
    }
    aux = BDutilizadores->next->next;
    free(BDutilizadores->next);
    BDutilizadores->next = aux;
}

void destroi_lista_utilizadores(lista_utilizadores *BDutilizadores) {
    lista_utilizadores *aux = BDutilizadores;
    while (BDutilizadores != NULL) {
        aux = BDutilizadores;
        BDutilizadores = BDutilizadores->next;
        elimina_utilizador(aux->pessoa);
        free(aux);
    }
}

lista_utilizadores *inicia_lista_utilizadores() {
    lista_utilizadores *aux = (lista_utilizadores *) malloc(sizeof(lista_utilizadores));
    aux->pessoa = 0;
    aux->next = NULL;
    return aux;
}

lista_utilizadores *cria_lista_utilizadores(utilizador *humano, lista_utilizadores *next) {
    lista_utilizadores *aux = (lista_utilizadores *) malloc(sizeof(lista_utilizadores));
    aux->pessoa = humano;
    aux->next = next;
    return aux;
}

utilizador *cria_utilizador(char *nome, char *address, int dia, int mes, int ano, int telefone,
                            lista_utilizadores *BD_utilizadores) { /*usar para registar utilizador*/
    utilizador *aux;
    long id;
    id = (long) BD_utilizadores->pessoa / sizeof(utilizador);
    BD_utilizadores->pessoa++;
    aux = (utilizador *) malloc(sizeof(utilizador));
    aux->nome_utilizador = strdup(
            nome); /*aloca memoria para string passada como argumento e devolve o seu ponteiro, guardando na estrutura local*/
    aux->morada = strdup(address);
    aux->id = id;
    aux->locais_escolhidos = inicia_lista_locais();
    aux->pdis_preferidos = inicia_lista_PDIs();
    aux->nascimento.dia = dia;
    aux->nascimento.mes = mes;
    aux->nascimento.ano = ano;
    aux->telefone = telefone;
    inserir_utilizador(BD_utilizadores, aux);
    return aux;
}

void inserir_utilizador(lista_utilizadores *lista, utilizador *humano) {
    lista_utilizadores *aux_lista;
    /**/
    while (lista->next != NULL && strcasecmp(humano->nome_utilizador, lista->next->pessoa->nome_utilizador) > 0) {
        lista = lista->next;
        /*desnecessario verificar se IDs iguais, porque gerados pelo programa*/
    }
    aux_lista = cria_lista_utilizadores(humano, lista->next);
    lista->next = aux_lista;
}

void imprime_lista_utilizadores(lista_utilizadores *lista) {
    while (lista->next != NULL) {
        lista = lista->next;
        printf("%d\t%s\t%s\t%d/%d/%d\t%d\n\n", lista->pessoa->id, lista->pessoa->nome_utilizador, lista->pessoa->morada,
               lista->pessoa->nascimento.dia, lista->pessoa->nascimento.mes, lista->pessoa->nascimento.mes,
               lista->pessoa->telefone);
    }
}


/*menus
 *
 *
 *
 *
 *
 * */

void menu_registo(lista_utilizadores *BDutilizadores) {
    char nome[BUFFER_SIZE], morada[BUFFER_SIZE];
    int dia, mes, ano, telefone;
    getchar();
    printf("Insira o seu nome:\n");
    fgets(nome, BUFFER_SIZE, stdin);
    remove_nova_linha(nome);
    printf("Insira a sua morada:\n");
    fgets(morada, BUFFER_SIZE, stdin);
    remove_nova_linha(morada);
    printf("Insira a sua data de nascimento (DD/MM/YYYY):\n");
    scanf("%d/%d/%d", &dia, &mes, &ano);
    printf("Insira o seu contacto telefonico\n");
    scanf("%d", &telefone);
    cria_utilizador(nome, morada, dia, mes, ano, telefone, BDutilizadores);
}

void menu_escolha_locais(lista_locais *BDlocais, lista_utilizadores *BDutilizadores) {
    int id_util, id_local, opcao = 1;
    while (opcao != 0) {
        printf("1- Inserir local\n");
        printf("2- Remover local\n");
        printf("3- Mostrar locais preferidos\n");
        printf("0- Sair");
        scanf("%d", &opcao);
        if (opcao == 0) break;
        printf("Qual o id do utilizador que deseja?\n");
        scanf("%d", &id_util);
        switch (opcao) {
            case 1:
                printf("Qual o id do local que deseja?\n");
                scanf("%d", &id_local);
                inserir_local(pesquisa_utilizador(BDutilizadores, id_util)->locais_escolhidos,
                              pesquisa_local(BDlocais, id_local));
                inserir_utilizador(pesquisa_local(BDlocais, id_local)->util,
                                   pesquisa_utilizador(BDutilizadores, id_util)); /*NAO TESTADO*/
                break;
            case 2:
                printf("Qual o id do local que deseja?\n");
                scanf("%d", &id_local);
                remove_local(pesquisa_utilizador(BDutilizadores, id_util)->locais_escolhidos, id_local);
                remove_utilizador(pesquisa_local(BDlocais, id_local)->util, id_util); /*NAO TESTADO*/
                break;
            case 3:
                imprime_lista_locais(pesquisa_utilizador(BDutilizadores, id_util)->locais_escolhidos);
                break;
            default:
                break;
        }
    }
}

void menu_escolha_pdis(lista_PDIs *BDPDIs, lista_utilizadores *BDutilizadores) {
    int id_util, id_PDI, opcao = 1;
    while (opcao != 0) {
        printf("1- Inserir PDI\n");
        printf("2- Remover PDI\n");
        printf("3- Mostrar PDIs preferidos\n");
        printf("0- Sair");
        scanf("%d", &opcao);
        if (opcao == 0) break;
        printf("Qual o id do utilizador que deseja?\n");
        scanf("%d", &id_util);
        switch (opcao) {
            case 1:
                printf("Qual o id do PDI que deseja?\n");
                scanf("%d", &id_PDI);
                inserir_PDI(pesquisa_utilizador(BDutilizadores, id_util)->pdis_preferidos,
                            pesquisa_PDI(BDPDIs, id_PDI));
                inserir_utilizador(pesquisa_PDI(BDPDIs, id_PDI)->util,
                                   pesquisa_utilizador(BDutilizadores, id_util)); /*NAO TESTADO*/
                break;
            case 2:
                printf("Qual o id do PDI que deseja?\n");
                scanf("%d", &id_PDI);
                remove_PDI(pesquisa_utilizador(BDutilizadores, id_util)->pdis_preferidos, id_PDI);
                remove_utilizador(pesquisa_PDI(BDPDIs, id_PDI)->util, id_util);/*NAO TESTADO*/
                break;
            case 3:
                imprime_lista_PDIs(pesquisa_utilizador(BDutilizadores, id_util)->pdis_preferidos);
                break;
            default:
                break;
        }
    }
}

void menu_listagens(lista_locais *BDlocais) {
    int opcao = 1;
    while (opcao != 0) {
        printf("1- Listagem por ordem alfabetica\n");
        printf("2- Listagem por popularidade\n");
        printf("0- Sair\n");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                ordena_local_alfabetico(BDlocais);
                imprime_local_pdis(BDlocais);
                break;
            case 2:
                ordena_local_popularidade(BDlocais);
                imprime_local_pdis(BDlocais);
                /*LISTAR LOCAIS POR POPULARIDADE*/
                break;
            default:
                break;

        }
    }
}

void menu_escolha_pdi_hot(lista_PDIs *BDPDIs, lista_utilizadores *BDutilizadores) {
    int opcao = 1, id_util, id_PDI;
    while (opcao != 0) {
        printf("1- Inserir PDI hot\n");
        printf("2- Remover PDI got\n");
        printf("3- Apresentar PDI hot\n");
        printf("0- Sair");
        scanf("%d", &opcao);
        if (opcao == 0) break;
        printf("Qual o id do utilizador que deseja?\n");
        scanf("%d", &id_util);
        switch (opcao) {
            case 1:
                printf("Qual o id do PDI que deseja?\n");
                scanf("%d", &id_PDI);
                pesquisa_utilizador(BDutilizadores, id_util)->hot = pesquisa_PDI(BDPDIs, id_PDI);
                inserir_utilizador(pesquisa_PDI(BDPDIs, id_PDI)->hot, pesquisa_utilizador(BDutilizadores, id_util));
                break;
            case 2:
                printf("Qual o id do PDI que deseja?\n");
                scanf("%d", &id_PDI);
                pesquisa_utilizador(BDutilizadores, id_util)->hot = NULL;
                remove_utilizador(pesquisa_PDI(BDPDIs, id_PDI)->hot, id_util);
                break;
            case 3:
                imprime_PDI_hot(id_util, BDutilizadores);
            default:
                break;
        }
    }
}

void ordena_local_popularidade(lista_locais *lista) {
    local *aux; lista_locais *inicial = lista->next;
    char swapped=1;/*saltar header*/
    while(swapped == 1) {
        swapped = 0;
        lista = inicial;
        while (lista->next != NULL) {
            if (conta_utilizadores(lista->loc->util) < conta_utilizadores(lista->next->loc->util)) {
                aux = lista->loc;
                lista->loc = lista->next->loc;
                lista->next->loc = aux;
                swapped = 1;
            }
            lista = lista->next; /*saltar header*/
        }
    }
}

void ordena_local_alfabetico(lista_locais *lista) {
    local *aux; lista_locais *inicial = lista->next;
    char swapped=1;/*saltar header*/
    while(swapped == 1) {
        swapped = 0;
        lista = inicial;
        while (lista->next != NULL) {
            if (strcasecmp(lista->loc->nome_local, lista->next->loc->nome_local) > 0) {
                aux = lista->loc;
                lista->loc = lista->next->loc;
                lista->next->loc = aux;
                swapped = 1;
            }
            lista = lista->next; /*saltar header*/
        }
    }
}

void ordena_PDIs_popularidade(lista_PDIs *BDPDIs) {
    lista_PDIs *aux;
    if (conta_utilizadores((BDPDIs->ponto_interesse->util)) <
        conta_utilizadores((BDPDIs->next->ponto_interesse->util))) {
        aux = BDPDIs;
        BDPDIs = BDPDIs->next;
        BDPDIs->next = aux;
    }
}

/*funcoes auxiliares
 *
 *
 * */
void remove_nova_linha(char *frase) {
    int len = strlen(frase);
    if (frase[len - 1] == '\n') {
        frase[len - 1] = '\0';
        if (frase[len - 2] == '\r') {
            frase[len - 2] = 0;
        }
    }
}

int conta_utilizadores(lista_utilizadores *lista) {
    int cnt = 0;
    for (cnt = 0; lista->next != NULL; cnt++) lista = lista->next;
    return cnt;
}

int conta_locais(lista_locais *lista) {
    int cnt = 0;
    for (cnt = 0; lista->next != NULL; cnt++) lista = lista->next;
    return cnt;
}

int conta_PDIs(lista_PDIs *lista) {
    int cnt = 0;
    for (cnt = 0; lista->next != NULL; cnt++) lista = lista->next;
    return cnt;
}