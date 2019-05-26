#include <stdio.h>
#include <strings.h>
#include "estruturas.h"

int main() {
    int opcao = 1;
    lista_locais *BDlocais, *BDlocaisPop;
    lista_PDIs *BDpdis;
    lista_utilizadores *BDutilizadores;
    BDlocais = inicia_lista_locais();
    BDlocaisPop = inicia_lista_locais();
    BDpdis = inicia_lista_PDIs();
    BDutilizadores = inicia_lista_utilizadores();
    /*carregar informacao do ficheiro para listas ligadas*/
    load_ficheiro_locais(BDlocais, BDpdis, BDlocaisPop);
    load_ficheiro_utilizador(BDutilizadores, BDpdis, BDlocais, BDlocaisPop);
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
        get_number(&opcao);
        switch (opcao) {
            case 1:
                menu_registo(BDutilizadores);
                break;
            case 2:
                menu_listagens(BDlocais, BDlocaisPop);
                break;
            case 3:
                imprime_lista_utilizadores(BDutilizadores);
                break;
            case 4:
                menu_escolha_locais(BDlocais, BDutilizadores, BDlocaisPop);
                break;
            case 5:
                menu_escolha_pdis(BDpdis, BDutilizadores);
                break;
            case 6:
                menu_escolha_pdi_hot(BDpdis, BDutilizadores);
                break;
            case 7:
                menu_viagem(BDutilizadores, BDpdis);
                break;
            default:
                break;
        }
    }
    /*desalocacao de memoria*/
    write_ficheiro_utilizador(BDutilizadores);
    destroi_lista_locais(BDlocais);
    elimina_lista_locais(BDlocaisPop);
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
local *pesquisa_local(lista_locais *lista, int id_a_encontrar) { /*devolve o nó da lista se for encontrado, ou NULL se inexistente*/
    lista = lista->next;
    while (lista != NULL && lista->loc->id != id_a_encontrar) {
        lista = lista->next;
    }
    if (lista == NULL) return NULL;
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
    elimina_lista_pdis(loc->pontospop);
    elimina_lista_utlizadores(loc->util);
    free(loc);
}

void destroi_lista_locais(lista_locais *BDlocais) {
    lista_locais *aux = BDlocais->next;
    free(BDlocais);
    BDlocais = aux;
    while (BDlocais != NULL) {
        aux = BDlocais;
        BDlocais = BDlocais->next;
        if (aux->loc != NULL)
            elimina_local(aux->loc);
        free(aux);
    }
}


lista_locais *inicia_lista_locais() {
    lista_locais *aux = (lista_locais *) calloc(sizeof(lista_locais), 1);
    if (aux == NULL) return NULL;
    aux->loc = 0;
    aux->next = NULL;
    return aux;
}

lista_locais *cria_lista_locais(local *loc, lista_locais *next) {
    lista_locais *aux = (lista_locais *) calloc(sizeof(lista_locais), 1);
    if (aux == NULL) return NULL;
    aux->loc = loc;
    aux->next = next;
    return aux;
}

local *cria_local(char *nome, lista_locais *BD_locais) { /*usar para carregar dados dos ficheiros*/
    local *aux;
    long id;
    id = (long) BD_locais->loc / sizeof(local);/*loc is pointer and is incremented every 24 bytes in a 64-bits machine*/
    BD_locais->loc++;
    aux = (local *) calloc(sizeof(local), 1);
    if (aux == NULL) return NULL;
    aux->nome_local = strdup(nome); /*aloca memoria para string passada como argumento e devolve o seu ponteiro, guardando na estrutura local*/
    aux->id = (int)id;
    aux->pontos = inicia_lista_PDIs();
    aux->pontospop = inicia_lista_PDIs();
    aux->util = inicia_lista_utilizadores();
    inserir_local(BD_locais, aux);
    return aux;
}


void inserir_local(lista_locais *lista, local *loc) {
    lista_locais *aux_lista;
    while (lista->next != NULL && lista->next->loc != NULL && strcasecmp(loc->nome_local, lista->next->loc->nome_local) > 0) {
        lista = lista->next;
    }
    aux_lista = cria_lista_locais(loc, lista->next);
    lista->next = aux_lista;
}


char remove_local(lista_locais *BDlocais, int id) {
    lista_locais *aux;
    if (pesquisa_local(BDlocais, id) == NULL) return 1;
    while (BDlocais->next != NULL && BDlocais->next->loc->id != id) {
        BDlocais = BDlocais->next;
    }
    aux = BDlocais->next->next;
    free(BDlocais->next);
    BDlocais->next = aux;
    return 0;
}

void imprime_lista_locais(lista_locais *lista) {
    while (lista->next != NULL) {
        lista = lista->next;
        printf("%d\n%s\n\n", lista->loc->id, lista->loc->nome_local);
    }
}

void imprime_local_pdis_alfabetica(lista_locais *BDlocais) {
    lista_PDIs *aux;
    while (BDlocais->next != NULL) {
        printf("%d\n%s\n", BDlocais->next->loc->id, BDlocais->next->loc->nome_local);
        aux = BDlocais->next->loc->pontos;
        while (aux->next != NULL) {
            printf("\t%d\n\t%s\n\t%s\n\t%s\n", aux->next->ponto_interesse->id, aux->next->ponto_interesse->nome_PDI, aux->next->ponto_interesse->descricao, aux->next->ponto_interesse->horario);
            aux = aux->next;
        }
        printf("\n");
        BDlocais = BDlocais->next;
    }
}

void imprime_local_pdis_pop(lista_locais *BDlocaispop) {
    lista_PDIs *aux;
    while (BDlocaispop->next != NULL) {
        printf("%d\n%s\n", BDlocaispop->next->loc->id, BDlocaispop->next->loc->nome_local);
        aux = BDlocaispop->next->loc->pontospop;
        while (aux->next != NULL) {
            printf("\t%d\n\t%s\n\t%s\n\t%s\n", aux->next->ponto_interesse->id, aux->next->ponto_interesse->nome_PDI, aux->next->ponto_interesse->descricao, aux->next->ponto_interesse->horario);
            aux = aux->next;
        }
        printf("\n");
        BDlocaispop = BDlocaispop->next;
    }
}

/*LISTAS PDIS
 *
 *
 *
 *
 * */
PDI *pesquisa_PDI(lista_PDIs *lista, int id_a_encontrar) { /*devolve o nó da lista se for encontrado, ou NULL se inexistente*/
    lista = lista->next;
    while (lista != NULL && lista->ponto_interesse->id != id_a_encontrar) {
        lista = lista->next;
    }
    if (lista == NULL) return NULL;
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
    elimina_lista_utlizadores(ponto->util);
    elimina_lista_utlizadores(ponto->hot);
    free(ponto);
}

void destroi_lista_PDIs(lista_PDIs *BDPDIs) {
    lista_PDIs *aux = BDPDIs->next;
    free(BDPDIs);
    BDPDIs = aux;
    while (BDPDIs != NULL) {
        aux = BDPDIs;
        BDPDIs = BDPDIs->next;
        if (aux->ponto_interesse != NULL)
            elimina_PDI(aux->ponto_interesse);
        free(aux);
    }
}

lista_PDIs *inicia_lista_PDIs() {
    lista_PDIs *aux = (lista_PDIs *) calloc(sizeof(lista_PDIs), 1);
    if (aux == NULL) return NULL;
    aux->ponto_interesse = 0;
    aux->next = NULL;
    return aux;
}

lista_PDIs *cria_lista_PDIs(PDI *ponto, lista_PDIs *next) {
    lista_PDIs *aux = (lista_PDIs *) calloc(sizeof(lista_PDIs), 1);
    if (aux == NULL) return NULL;
    aux->ponto_interesse = ponto;
    aux->next = next;
    return aux;
}

PDI *cria_PDI(char *nome, char *descricao, char *horario, lista_PDIs *BD_PDIs, local *loc) { /*usar para carregar dados dos ficheiros*/
    PDI *aux;
    long id;
    id = (long) BD_PDIs->ponto_interesse / sizeof(PDI);
    BD_PDIs->ponto_interesse++;
    aux = (PDI *) calloc(sizeof(PDI), 1);
    if (aux == NULL) return NULL;
    aux->nome_PDI = strdup(nome); /*aloca memoria para string passada como argumento e devolve o seu ponteiro, guardando na estrutura local*/
    aux->descricao = strdup(descricao);
    aux->horario = strdup(horario);
    aux->id = (int) id;
    aux->loc = loc;
    aux->util = inicia_lista_utilizadores();
    aux->hot = inicia_lista_utilizadores();
    inserir_PDI(BD_PDIs, aux);
    inserir_PDI(loc->pontos, aux);
    inserir_PDI(loc->pontospop, aux);
    return aux;
}

void inserir_PDI(lista_PDIs *lista, PDI *ponto) {
    lista_PDIs *aux_lista;
    while (lista->next != NULL && lista->next->ponto_interesse != NULL && strcasecmp(ponto->nome_PDI, lista->next->ponto_interesse->nome_PDI) > 0) {
        lista = lista->next;
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

void imprime_PDI_hot(int id_util, lista_utilizadores *BDutilizadores) {
    utilizador *util;
    util = pesquisa_utilizador(BDutilizadores, id_util);
    printf("%d\n%s\n%s\n%s\n\n", util->hot->id, util->hot->nome_PDI, util->hot->descricao, util->hot->horario);
}


char remove_PDI(lista_PDIs *BDpdis, int id) {
    lista_PDIs *aux;
    if (pesquisa_PDI(BDpdis, id) == NULL) return 1;
    while (BDpdis->next != NULL && BDpdis->next->ponto_interesse->id != id) {
        BDpdis = BDpdis->next;
    }
    aux = BDpdis->next->next;
    free(BDpdis->next);
    BDpdis->next = aux;
    return 0;
}

/*lista de utilizadores
 *
 *
 *
 * */

utilizador *pesquisa_utilizador(lista_utilizadores *lista, int id_a_encontrar) { /*devolve o nó da lista se for encontrado, ou NULL se inexistente*/
    lista = lista->next;
    while (lista != NULL && lista->pessoa->id != id_a_encontrar) {
        lista = lista->next;
    }
    if (lista == NULL) return NULL;
    return lista->pessoa;
}

void elimina_lista_utlizadores(lista_utilizadores *lista) {
    lista_utilizadores *aux;
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


char remove_utilizador(lista_utilizadores *lista, int id) {
    lista_utilizadores *aux;
    while (lista->next != NULL && lista->next->pessoa->id != id) {
        lista = lista->next;
    }
    aux = lista->next->next;
    free(lista->next);
    lista->next = aux;
    return 0;
}

void destroi_lista_utilizadores(lista_utilizadores *BDutilizadores) {
    lista_utilizadores *aux = BDutilizadores->next;
    free(BDutilizadores);
    BDutilizadores = aux;
    while (BDutilizadores != NULL) {
        aux = BDutilizadores;
        BDutilizadores = BDutilizadores->next;
        if (aux->pessoa != NULL)
            elimina_utilizador(aux->pessoa);
        free(aux);
    }
}

lista_utilizadores *inicia_lista_utilizadores() {
    lista_utilizadores *aux = (lista_utilizadores *) calloc(sizeof(lista_utilizadores), 1);
    if (aux == NULL) return NULL;
    aux->pessoa = 0;
    aux->next = NULL;
    return aux;
}

lista_utilizadores *cria_lista_utilizadores(utilizador *humano, lista_utilizadores *next) {
    lista_utilizadores *aux = (lista_utilizadores *) calloc(sizeof(lista_utilizadores), 1);
    if (aux == NULL) return NULL;
    aux->pessoa = humano;
    aux->next = next;
    return aux;
}

utilizador *cria_utilizador(char *nome, char *address, int dia, int mes, int ano, int telefone, lista_utilizadores *BD_utilizadores) { /*usar para registar utilizador*/
    utilizador *aux;
    long id;
    id = (long) BD_utilizadores->pessoa / sizeof(utilizador);
    BD_utilizadores->pessoa++;
    aux = (utilizador *) calloc(sizeof(utilizador), 1);
    if (aux == NULL) return NULL;
    aux->nome_utilizador = strdup(nome); /*aloca memoria para string passada como argumento e devolve o seu ponteiro, guardando na estrutura local*/
    aux->morada = strdup(address);
    aux->id = (int) id;
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
    while (lista->next != NULL && lista->pessoa != NULL && strcasecmp(humano->nome_utilizador, lista->next->pessoa->nome_utilizador) > 0) {
        lista = lista->next;
    }
    aux_lista = cria_lista_utilizadores(humano, lista->next);
    lista->next = aux_lista;
}

void imprime_lista_utilizadores(lista_utilizadores *lista) {
    while (lista->next != NULL) {
        lista = lista->next;
        printf("%d\t%s\t%s\t%d/%d/%d\t%d\n\n", lista->pessoa->id, lista->pessoa->nome_utilizador, lista->pessoa->morada, lista->pessoa->nascimento.dia, lista->pessoa->nascimento.mes, lista->pessoa->nascimento.ano, lista->pessoa->telefone);
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
    char erro_data, erro_telefone;
    char nome[BUFFER_SIZE], morada[BUFFER_SIZE];
    int dia, mes, ano, telefone;
    printf("Insira o seu nome:\n");
    fgets(nome, BUFFER_SIZE, stdin);
    remove_nova_linha(nome);
    printf("Insira a sua morada:\n");
    fgets(morada, BUFFER_SIZE, stdin);
    remove_nova_linha(morada);
    do {
        printf("Insira o seu dia de aniversario:\n");
        get_number(&dia);
        printf("Insira o seu mes de aniversario:\n");
        get_number(&mes);
        printf("Insira o ano em que nasceu:\n");
        get_number(&ano);
        erro_data = verifica_data(dia, mes, ano);
        if (erro_data) data_erros(erro_data);
    } while (erro_data);
    do {
        printf("Insira o seu contacto telefonico\n");
        get_number(&telefone);
        erro_telefone = verifica_telefone(telefone);
    } while (erro_telefone != 0);
    cria_utilizador(nome, morada, dia, mes, ano, telefone, BDutilizadores);
}

void menu_escolha_locais(lista_locais *BDlocais, lista_utilizadores *BDutilizadores, lista_locais *BDlocaisPop) {
    int id_util, id_local, opcao = 1;
    char erro;
    while (opcao != 0) {
        printf("1- Inserir local\n");
        printf("2- Remover local\n");
        printf("3- Mostrar locais preferidos\n");
        printf("0- Sair");
        get_number(&opcao);
        if (opcao == 0) break;
        printf("Qual o id do utilizador que deseja?\n");
        get_number(&id_util);
        if (!pesquisa_utilizador(BDutilizadores, id_util)) {
            printf("Utilizador inxesistente\n");
            return;
        }
        switch (opcao) {
            case 1:
                if (conta_locais(pesquisa_utilizador(BDutilizadores, id_util)->locais_escolhidos) < 3) {
                    printf("Qual o id do local que deseja?\n");
                    get_number(&id_local);
                    if (!pesquisa_local(BDlocais, id_local)) {
                        printf("Local inexistente\n");
                        return;
                    }
                    inserir_local(pesquisa_utilizador(BDutilizadores, id_util)->locais_escolhidos, pesquisa_local(BDlocais, id_local));
                    inserir_utilizador(pesquisa_local(BDlocais, id_local)->util, pesquisa_utilizador(BDutilizadores, id_util));
                    ordena_localpop(id_local, BDlocaisPop);
                } else {
                    printf("Nao pode escolher mais locais\n");
                }
                break;
            case 2:
                if (conta_locais(pesquisa_utilizador(BDutilizadores, id_util)->locais_escolhidos) > 0) {
                    printf("Qual o id do local que deseja?\n");
                    get_number(&id_local);
                    if (!pesquisa_local(BDlocais, id_local)) {
                        printf("Local inexistente\n");
                        return;
                    }
                    erro = remove_local(pesquisa_utilizador(BDutilizadores, id_util)->locais_escolhidos, id_local);
                    if (!erro) {
                        remove_utilizador(pesquisa_local(BDlocais, id_local)->util, id_util); /*NAO TESTADO*/
                        ordena_localpop(id_local, BDlocaisPop);
                    } else printf("Local inexixtente\n");
                }
                break;;
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
        get_number(&opcao);
        if (opcao == 0) break;
        printf("Qual o id do utilizador que deseja?\n");
        get_number(&id_util);
        switch (opcao) {
            case 1:
                printf("Qual o id do PDI que deseja?\n");
                get_number(&id_PDI);
                inserir_PDI(pesquisa_utilizador(BDutilizadores, id_util)->pdis_preferidos, pesquisa_PDI(BDPDIs, id_PDI));
                inserir_utilizador(pesquisa_PDI(BDPDIs, id_PDI)->util, pesquisa_utilizador(BDutilizadores, id_util));
                ordena_PDIpop(id_PDI, pesquisa_PDI(BDPDIs, id_PDI)->loc->pontospop);
                break;
            case 2:
                printf("Qual o id do PDI que deseja?\n");
                get_number(&id_PDI);
                if (pesquisa_PDI(pesquisa_utilizador(BDutilizadores, id_util)->pdis_preferidos, id_PDI) == NULL) {
                    printf("Sem PDI selecionado\n");
                } else {
                    remove_PDI(pesquisa_utilizador(BDutilizadores, id_util)->pdis_preferidos, id_PDI);
                    remove_utilizador(pesquisa_PDI(BDPDIs, id_PDI)->util, id_util);
                    ordena_PDIpop(id_PDI, pesquisa_PDI(BDPDIs, id_PDI)->loc->pontospop);
                }
                break;
            case 3:
                if (pesquisa_utilizador(BDutilizadores, id_util)->pdis_preferidos != NULL) imprime_lista_PDIs(pesquisa_utilizador(BDutilizadores, id_util)->pdis_preferidos);
                else printf("Sem PDI selecionado\n");
                break;
            default:
                break;
        }
    }
}

void menu_listagens(lista_locais *BDlocais, lista_locais *BDlocaisPop) {
    int opcao = 1;
    while (opcao != 0) {
        printf("1- Listagem por ordem alfabetica\n");
        printf("2- Listagem por popularidade\n");
        printf("0- Sair\n");
        get_number(&opcao);
        switch (opcao) {
            case 1:
                imprime_local_pdis_alfabetica(BDlocais);
                break;
            case 2:
                imprime_local_pdis_pop(BDlocaisPop);
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
        printf("2- Remover PDI hot\n");
        printf("3- Apresentar PDI hot\n");
        printf("0- Sair");
        get_number(&opcao);
        if (opcao == 0) break;
        printf("Qual o id do utilizador que deseja?\n");
        get_number(&id_util);
        switch (opcao) {
            case 1:
                printf("Qual o id do PDI que deseja?\n");
                get_number(&id_PDI);
                pesquisa_utilizador(BDutilizadores, id_util)->hot = pesquisa_PDI(BDPDIs, id_PDI);
                inserir_utilizador(pesquisa_PDI(BDPDIs, id_PDI)->hot, pesquisa_utilizador(BDutilizadores, id_util));
                break;
            case 2:
                if (pesquisa_utilizador(BDutilizadores, id_util)->hot == NULL) {
                    printf("Sem PDI hot selecionado\n");
                } else {
                    remove_utilizador(pesquisa_PDI(BDPDIs, pesquisa_utilizador(BDutilizadores, id_util)->hot->id)->hot, id_util);
                    pesquisa_utilizador(BDutilizadores, id_util)->hot = NULL;
                }
                break;
            case 3:
                if (pesquisa_utilizador(BDutilizadores, id_util)->hot != NULL) imprime_PDI_hot(id_util, BDutilizadores);
                else printf("Sem PDI hot selecionado\n");
            default:
                break;
        }
    }
}

void menu_viagem(lista_utilizadores *BDutilizadores, lista_PDIs *BDPDIs) {/*NAO TESTADO ATE FICHEIRO INICIAL LIDO*/
    int id, i;
    lista_PDIs *viagem, *viagem_cabecalho;
    lista_locais *aux;
    printf("Qual e o id do utilizador que deseja?\n");
    get_number(&id);
    if (conta_locais(pesquisa_utilizador(BDutilizadores, id)->locais_escolhidos) != 3) {
        printf("Este utilizador ainda não escolheu 3 locais preferidos\n");
        return;
    }
    viagem_cabecalho = constroi_viagem(id, BDutilizadores);
    viagem = viagem_cabecalho->next;
    aux = pesquisa_utilizador(BDutilizadores, id)->locais_escolhidos->next;
    for (id = 0; id < 3; id++) {
        printf("%d\n%s\n", aux->loc->id, aux->loc->nome_local);
        for (i = 0; i < 3; i++) {
            printf("\t%d\n\t%s\n\t%s\n\t%s\n", viagem->ponto_interesse->id, viagem->ponto_interesse->nome_PDI, viagem->ponto_interesse->descricao, viagem->ponto_interesse->horario);
            viagem = viagem->next;
        }
        aux = aux->next;
    }
    printf("%f\n", percentagem_local_preferido(pesquisa_utilizador(BDutilizadores, id)->locais_escolhidos, BDutilizadores) + percentagem_PDI_hot(BDutilizadores, viagem_cabecalho) + percentagem_preferencias_PDI(BDPDIs, viagem_cabecalho) / 3);
    destroi_lista_PDIs(viagem_cabecalho);
}

/*funcoes auxiliares
 *
 *
 * */
void remove_nova_linha(char *frase) {
    int len = (int)strlen(frase);
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

lista_PDIs *constroi_viagem(int id_util, lista_utilizadores *BDutilizadores) {
    int i;
    utilizador *humano;
    lista_locais *locais;
    local *loc;
    lista_PDIs *aux, *resultado;
    humano = pesquisa_utilizador(BDutilizadores, id_util);
    resultado = inicia_lista_PDIs();
    locais = humano->locais_escolhidos;
    
    for (i = 1; i < 4; i++) {
        locais = locais->next;
        loc = locais->loc;
        aux = loc->pontospop->next;
        while (aux != NULL && conta_PDIs(resultado) < 3 * i) { /*Encontra o hot se existir e adiciona-o a lista*/
            if (aux->ponto_interesse == humano->hot) {
                inserir_PDI(resultado, aux->ponto_interesse);
                break;
            }
            aux = aux->next;
        }
        aux = loc->pontospop->next;
        while (aux != NULL && conta_PDIs(resultado) < 3 * i) { /*Verifica se PDI ja esta na lista resultado e se pertence aos preferidos do utilizador*/
            if (pesquisa_PDI(resultado, aux->ponto_interesse->id) == NULL && pesquisa_PDI(humano->pdis_preferidos, aux->ponto_interesse->id) != NULL) {
                inserir_PDI(resultado, aux->ponto_interesse);
            }
            aux = aux->next;
        }
        aux = loc->pontospop->next;
        while (aux != NULL && conta_PDIs(resultado) < 3 * i) { /*Verifica se os pontos ja estao na lista resultado e adiciona se nao*/
            if (pesquisa_PDI(resultado, aux->ponto_interesse->id) == NULL) {
                inserir_PDI(resultado, aux->ponto_interesse);
            }
            aux = aux->next;
        }
    }
    return resultado;
}

float percentagem_local_preferido(lista_locais *locais_util, lista_utilizadores *BDutilizadores) { /*nao testado*/
    int cnt = 0, total = 0;
    lista_utilizadores *lista = BDutilizadores;
    lista_locais *locais_escolhidos;
    BDutilizadores = BDutilizadores->next;
    locais_util = locais_util->next;
    while (BDutilizadores != NULL) { /*ciclo exterior percorre cada utilizador*/
        locais_escolhidos = BDutilizadores->pessoa->locais_escolhidos->next;/*locais escolhidos é no da lista de locais de cada utilizador*/
        while (locais_escolhidos != NULL) {
            if (locais_util->loc == locais_escolhidos->loc || locais_util->next->loc == locais_escolhidos->loc || locais_util->next->next->loc == locais_escolhidos->loc) {
                cnt++;
                break; /*caso ja tenha encontrado local em comum, o ciclo para*/
            }
            locais_escolhidos = locais_escolhidos->next;
        }
        BDutilizadores = BDutilizadores->next;
    }
    total = conta_utilizadores(lista);
    return (float) cnt / (float) total * 100;
}

float percentagem_PDI_hot(lista_utilizadores *BDutilizadores, lista_PDIs *viagem) {
    int cnt = 0, total;
    lista_utilizadores *lista = BDutilizadores;
    BDutilizadores = BDutilizadores->next;
    while (BDutilizadores != NULL) {
        if (BDutilizadores->pessoa->hot != NULL && pesquisa_PDI(viagem, BDutilizadores->pessoa->hot->id) != NULL) {
            cnt++;
        }
        BDutilizadores = BDutilizadores->next;
    }
    total = conta_utilizadores(lista);
    return (float) cnt / (float) total * 100;
}

float percentagem_preferencias_PDI(lista_PDIs *BDPDIs, lista_PDIs *viagem) {
    int cnt_pdis_viagem = 0, cnt_total_pdis = 0;
    BDPDIs = BDPDIs->next;
    viagem = viagem->next;
    while (viagem != NULL) {
        cnt_pdis_viagem += conta_PDIs(viagem);
        viagem = viagem->next;
    }
    while (BDPDIs != NULL) {
        cnt_total_pdis += conta_PDIs(BDPDIs);
        BDPDIs = BDPDIs->next;
    }
    return (float) cnt_pdis_viagem / (float) cnt_total_pdis * 100;
}

void clear_input() {
    char aux;
    do {
        aux = (char) getchar();
    } while (aux != '\n');
}

char input_number(int *number) {
    char aux[INT_DIM];
    char *safe;
    fgets(aux, INT_DIM, stdin);
    if (aux[strlen(aux) - 1] != '\n') clear_input(); /*if last character was not '\n' must clear input*/
    *number = (int) strtol(aux, &safe, 10);
    if (strlen(aux) <= 1 || !strcmp(safe, aux)) return 0; /*returns 0 if input incorrect*/
    if (!strcmp(safe, "\n")) return 1; /*returns 1 if input was just a number*/
    if (!strcmp(safe, "")) return 2; /*returns 2 if input was too long*/
    return -1; /*returns 0 if input started by a number but had more than it*/
}

void input_errors(char erro) {
    switch (erro) {
        case -1:
            printf("Caracteres invalidos no final do input\n");
            break;
        case 0:
            printf("O input nao e um numero\n");
            break;
        case 1:
            /*printf("Imput correto\n");*/
            break;
        case 2:
            printf("Input demasiado longo. O maior numero e 999.999.999\n");
            break;
        default:
            break;
    }
}

void get_number(int *num) {
    char erro;
    do {
        erro = input_number(num);
        input_errors(erro);
    } while (erro != 1);
}

char verifica_data(int dia, int mes, int ano) {
    int dia_max;
    if (ano > ANO_ATUAL || ano < ANO_INICIAL) return 3;
    if (mes > 12 || mes < 1) return 2;
    if (dia < 1) return 1;
    dia_max = 31;
    if (mes == 4 || mes == 6 || mes == 9 || mes == 11) dia_max = 30;
    if (mes == 2) {
        if (!(ano % 4)) dia_max = 29;
        else dia_max = 28;
    }
    if (dia > dia_max) return 1;
    return 0;
}

void data_erros(char erro) {
    switch (erro) {
        case 1:
            printf("Dia inexistente no respetivo mes\n");
            break;
        case 2:
            printf("Mes inxesistente\n");
            break;
        case 3:
            printf("Ano invalido (1800<ano<2019)\n");
            break;
        default:
            break;
    }
}

char verifica_telefone(int telefone) {
    if (telefone < N_TEL_MIN || (telefone > N_TEL_MED_MIN && telefone < N_TEL_MED_MAX) || telefone > N_TEL_MAX) {
        printf("Numero de telefone invalido\n");
        return 1;
    }
    return 0;
}

/*FICHEIROS*/
void load_ficheiro_locais(lista_locais *BDlocais, lista_PDIs *BDpdis, lista_locais *BDlocaisPop) {
    FILE *fp;
    fp = fopen(NOME_F_LOCAIS, "r");
    int i, n_pdis;
    char linha[BUFFER_SIZE];
    char cidade[BUFFER_SIZE];
    char nome_pdi[BUFFER_SIZE];
    char descricao[BUFFER_SIZE];
    char horario[BUFFER_SIZE];
    local *loc;
    if (fp == NULL) printf("Ficheiro Inexistente");
    else {
        while (!feof(fp)) {
            /*enquanto nao chegar ao fim ler o ficheiro*/
            fgets(linha, 100, fp);
            strcpy(cidade, strtok(linha, "|"));
            n_pdis = atoi(strtok(NULL, "\n"));
            loc = cria_local(cidade, BDlocais);
            inserir_local(BDlocaisPop, loc); /*INSTRUCAO PARA LISTA DE POPULARIDADE*/
            i = 0;
            while (i != n_pdis) {
                fgets(linha, 1000, fp);
                strcpy(nome_pdi, strtok(linha, "|"));
                strcpy(descricao, strtok(NULL, "|"));
                strcpy(horario, strtok(NULL, "\n"));
                cria_PDI(nome_pdi, descricao, horario, BDpdis, loc);
                i++;
            }
            fgets(linha, 100, fp);
        }
        fclose(fp);
    }
}

void write_ficheiro_utilizador(lista_utilizadores *BDutilizadores) {
    FILE *fp;
    lista_locais *aux_locais;
    lista_PDIs *aux_pdis;
    fp = fopen(NOME_F_UTILIZADOR, "w");
    fprintf(fp, "%d\n", conta_utilizadores(BDutilizadores));
    BDutilizadores = BDutilizadores->next;
    while (BDutilizadores != NULL) {
        /*info basica*/fprintf(fp, "%s|%s|%d/%d/%d|%d|", BDutilizadores->pessoa->nome_utilizador, BDutilizadores->pessoa->morada, BDutilizadores->pessoa->nascimento.dia, BDutilizadores->pessoa->nascimento.mes, BDutilizadores->pessoa->nascimento.ano, BDutilizadores->pessoa->telefone);
        /*pdi hot*/if (BDutilizadores->pessoa->hot != NULL)
            fprintf(fp, "%d\n", BDutilizadores->pessoa->hot->id);
        else fprintf(fp, "-1\n");
        /*LOCAIS PREFERIDOS*/
        aux_locais = BDutilizadores->pessoa->locais_escolhidos->next;
        if (aux_locais == NULL) fprintf(fp, "-1");
        while (aux_locais != NULL) {
            fprintf(fp, "%d", aux_locais->loc->id);
            if (aux_locais->next != NULL) fprintf(fp, "|");
            aux_locais = aux_locais->next;
        }
        fprintf(fp, "\n");
        /*pdis preferidos*/
        aux_pdis = BDutilizadores->pessoa->pdis_preferidos->next;
        if (aux_pdis == NULL) fprintf(fp, "-1");
        while (aux_pdis != NULL) {
            fprintf(fp, "%d", aux_pdis->ponto_interesse->id);
            if (aux_pdis->next != NULL) fprintf(fp, "|");
            aux_pdis = aux_pdis->next;
        }
        fprintf(fp, "\n");
        BDutilizadores = BDutilizadores->next;
    }
    fclose(fp);
}

void load_ficheiro_utilizador(lista_utilizadores *BDutilizadores, lista_PDIs *BDPDIs, lista_locais *BDlocais, lista_locais *BDlocaisPop) {
    FILE *fp;
    int n;
    int i;
    char verify;
    char buffer[BUFFER_SIZE], *auxbuffer;
    char nome[BUFFER_SIZE], morada[BUFFER_SIZE];
    int dia, mes, ano, telefone;
    int id_aux;
    utilizador *util;
    fp = fopen(NOME_F_UTILIZADOR, "r");
    if (fp == NULL) return;
    fscanf(fp, "%d%c", &n, &verify);
    if (verify == '\r') fscanf(fp, "%c", &verify);
    for (i = 0; i < n; i++) {
        /*cria_utilizador com informacao base*/
        fgets(buffer, BUFFER_SIZE, fp);
        strcpy(nome, strtok(buffer, "|"));
        strcpy(morada, strtok(NULL, "|"));
        auxbuffer = strtok(NULL, "|");
        sscanf(auxbuffer, "%d/%d/%d", &dia, &mes, &ano);
        auxbuffer = strtok(NULL, "|");
        sscanf(auxbuffer, "%d", &telefone);
        util = cria_utilizador(nome, morada, dia, mes, ano, telefone, BDutilizadores); /*guarda id de utilizador mais recentemente criado*/
        /*adiciona pdi hot ao utilizador*/
        auxbuffer = strtok(NULL, "\n");
        sscanf(auxbuffer, "%d", &id_aux);
        if (id_aux != -1) {
            util->hot = pesquisa_PDI(BDPDIs, id_aux);
            inserir_utilizador(pesquisa_PDI(BDPDIs, id_aux)->hot, util);
        }
        /*insere_locais preferidos do utilizador*/
        do {
            fscanf(fp, "%d%c", &id_aux, &verify);
            if (verify == '\r') fscanf(fp, "%c", &verify);
            if (id_aux == -1) break;
            inserir_local(util->locais_escolhidos, pesquisa_local(BDlocais, id_aux));
            inserir_utilizador(pesquisa_local(BDlocais, id_aux)->util, util);
            ordena_localpop(id_aux, BDlocaisPop);
        } while (verify != '\n' && verify != '\r');
        /*insere pdis preferidos do utilizador*/
        do {
            fscanf(fp, "%d%c", &id_aux, &verify);
            if (verify == '\r') fscanf(fp, "%c", &verify);
            if (id_aux == -1) break;
            inserir_PDI(util->pdis_preferidos, pesquisa_PDI(BDPDIs, id_aux));
            inserir_utilizador(pesquisa_PDI(BDPDIs, id_aux)->util, util);
            ordena_PDIpop(id_aux, pesquisa_PDI(BDPDIs, id_aux)->loc->pontospop);
        } while (verify != '\n' && verify != '\r');
    }
    fclose(fp);
}

void ordena_localpop(int id_local, lista_locais *BDlocaisPop) {
    lista_locais *posicao_correta, *aux;
    posicao_correta = BDlocaisPop;
    while (BDlocaisPop->next != NULL && BDlocaisPop->next->loc->id != id_local) {
        BDlocaisPop = BDlocaisPop->next;
    }
    while (posicao_correta->next != NULL && conta_utilizadores(posicao_correta->next->loc->util) > conta_utilizadores(BDlocaisPop->next->loc->util)) {
        posicao_correta = posicao_correta->next;
    }
    if (BDlocaisPop == posicao_correta) return;
    aux = BDlocaisPop->next->next;
    BDlocaisPop->next->next = posicao_correta->next;
    posicao_correta->next = BDlocaisPop->next;
    BDlocaisPop->next = aux;
}

void ordena_PDIpop(int id_pdi, lista_PDIs *lista) {
    lista_PDIs *posicao_correta, *aux;
    posicao_correta = lista;
    while (lista->next != NULL && lista->next->ponto_interesse->id != id_pdi) {
        lista = lista->next;
    }
    while (posicao_correta->next != NULL && conta_utilizadores(posicao_correta->next->ponto_interesse->util) > conta_utilizadores(lista->next->ponto_interesse->util)) {
        posicao_correta = posicao_correta->next;
    }
    if (lista == posicao_correta) return;
    aux = lista->next->next;
    lista->next->next = posicao_correta->next;
    posicao_correta->next = lista->next;
    lista->next = aux;
}