#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_MATERIA_PRIMA 1000 //definindo o valor máximo de matéria prima do deposito
#define CAPACIDADE_MAXIMA_CANETAS 5//definindo a capacidade máxima de canetas
#define MAX_CANETAS_PARA_COMPRA 10//definindo o valor máximo de canetas para compra

// Variáveis globais
//7 argumentos de entrada
//TODO CRIAR MUTEX PARA AS VARIAVEIS GLOBAIS EM TODO
int materia_prima_disponivel; //TODOqtde matéria prima existente no depósito para fabricação das canetas
int qtde_enviada_interacao; //qtde unidades enviadas por interação do Dep. Mat. Prima à célula de fabricação das canetas
int tempo_entre_envio;// tempo em segundos entre cada envio de Matéria Prima às células de fabricação
int tempo_fabricar_caneta;// tempo em segundos para fabricar UMA caneta
int qtde_maxima_canetas;//TODOqtde máxima de canetas que podem ser armazenadas no depósito de canetas simultaneamente
int qtde_comprada_interacao;//qtde de canetas compradas a cada interação pelo comprador
int tempo_espera_compra;// tempo em segundos de espera entre as compras de canetas
int canetas_disponiveis = 0;//TODOMutex

// Declaração de semaforos e variaveis de condição
sem_t sem_materia_prima, sem_canetas;//TODO Entender quem mexe nesse semaforos

pthread_cond_t cond_materia_prima, cond_canetas;
pthread_mutex_t mutex;

// Prototipos das funções
void *criador(void *args);
void *deposito_materia_prima(void *args);
void *celula_fabricacao_canetas(void *args);
void *controle(void *args);
void *deposito_canetas(void *args);
void *comprador(void *args);

int main(int argc, char *argv[]) {
    if (argc != 8) {
        fprintf(stderr, "Uso: %s <qtde_materia_prima> <qtde_enviada_interacao> <tempo_entre_envio> <tempo_fabricar_caneta> <qtde_maxima_canetas> <qtde_comprada_interacao> <tempo_espera_compra>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Ler os argumentos de entrada
    materia_prima_disponivel = atoi(argv[1]);
    qtde_enviada_interacao = atoi(argv[2]);
    tempo_entre_envio = atoi(argv[3]);
    tempo_fabricar_caneta = atoi(argv[4]);
    qtde_maxima_canetas = atoi(argv[5]);
    qtde_comprada_interacao = atoi(argv[6]);
    tempo_espera_compra = atoi(argv[7]);

    // Chamar a função criador
    criador(NULL);

    return 0;
}

// TODO Colocar todas as saidas no criador
void *criador(void *args) { //Rank 0
    pthread_t threads[5];
    pthread_create(&threads[0], NULL, deposito_materia_prima, NULL);
    pthread_create(&threads[1], NULL, celula_fabricacao_canetas, NULL);
    pthread_create(&threads[2], NULL, controle, NULL);
    pthread_create(&threads[3], NULL, deposito_canetas, NULL);
    pthread_create(&threads[4], NULL, comprador, NULL);
    // Inicializar semaforos e variaveis de condição
    sem_init(&sem_materia_prima, 0, 0);
    sem_init(&sem_canetas, 0, 0);
    pthread_cond_init(&cond_materia_prima, NULL);
    pthread_cond_init(&cond_canetas, NULL);
    pthread_mutex_init(&mutex, NULL);

    // Aguardar a finalização das threads
    for (int i = 0; i <5; i++) {
        pthread_join(threads[i], NULL);
    }
    while(1){//!Existe um caso onde, após a matéria prima acabar, o estoque de canetas pode zerar e programa ser encerrado MESMO QUE tenham canetas sendo produzidas na fábrica
        if(materia_prima_disponivel == 0 && canetas_disponiveis == 0){
            printf("Fim da execução\n");
            exit(EXIT_SUCCESS);
    }
    }
    // Encerrar aplicação
    pthread_exit(NULL);
    
}

void *deposito_materia_prima(void *args) { //rank 1
    while (1) {
        // Produzir materia prima
        sleep(tempo_entre_envio);
        if(materia_prima_disponivel == 0){
            pthread_cond_wait(&cond_materia_prima, &mutex);
        }
        pthread_mutex_lock(&mutex);
        //TODO transferir materia prima caso controle determine 
        pthread_mutex_unlock(&mutex);        
    }
}
// TODO Consumir uma quantidade de materia prima a cada vez que um caneta é produzida, caso não tenha materia prima, encerrar o programa 
void *celula_fabricacao_canetas(void *args) { //rank 2
    while (1) {
        // Aguardar materia prima disponivel
        sem_wait(&sem_materia_prima);//?Quem altera esse semaforo? Deveria ser deposito materia prima

        // Produzir canetas
        sleep(tempo_fabricar_caneta);

        // Adicionar canetas ao deposito
        pthread_mutex_lock(&mutex);
        while (canetas_disponiveis == qtde_maxima_canetas) {//!Isso vai no controle, ta dormindo dentro do mutex
            // Capacidade maxima atingida, aguardar espaço no deposito
            pthread_cond_wait(&cond_canetas, &mutex);
        }
        canetas_disponiveis++;
        materia_prima_disponivel -= qtde_enviada_interacao;//!QUem diminui a qtd de materia prima disponivel deveria ser o estque de materia prima
        printf("Caneta produzida: %d\n", canetas_disponiveis);
        sem_post(&sem_canetas);
        pthread_mutex_unlock(&mutex);
    }
}

void *controle(void *args) { //rank 3
    while(1){
       // determina se o deposito de materia prima rank 1 pode transferir materia prima para a celula de fabricacao de canetas rank 2
       // determina se a celula de fabricacao de canetas rank 2 pode produzir canetas
       // recebe do deposito a quantidade de canetas

        if (materia_prima_disponivel >= qtde_enviada_interacao) {//! O controle deveria estar verificando e alterando variaveis condicionais para controlar
            // Materia prima disponivel, transferir para a celula de fabricacao
            pthread_mutex_lock(&mutex);
            materia_prima_disponivel -= qtde_enviada_interacao;//!Porque ta aq? Ele deveria so acordar o estoque
            printf("Materia prima transferida: %d\n", materia_prima_disponivel);//!
            sem_post(&sem_materia_prima);
            pthread_cond_signal(&cond_materia_prima);
            pthread_mutex_unlock(&mutex);
        }
        if(canetas_disponiveis < qtde_maxima_canetas){ //! O controle deveria estar verificando e alterando variaveis condicionais para controlar
            // Produzir canetas
            sleep(tempo_fabricar_caneta);

            // Adicionar canetas ao deposito
            pthread_mutex_lock(&mutex);
            canetas_disponiveis++;
            printf("Caneta produzida: %d\n", canetas_disponiveis);
            sem_post(&sem_canetas);
            pthread_mutex_unlock(&mutex);
        }// TODO COMO CONSERTAR: VERIFICAR if(canetas_disponiveis >= qtde_maxima_canetas) com else if caso > printa erro, paralisar envio,fabricacao e envio p/ armazenagem
        //TODO criar o else para paralizar o envio de materia e fabricacao de canetas e envio p/ armazenamento
    }
}


void *deposito_canetas(void *args) { //rank 4//!O DEPOSITO A CADA ITERAÇÃO DETERMINA PARA O CONTROLE A QUANTIDADE DE SLOTS DISPONIVEIS, RECEBER PEDIDOS DO COMPRADOR, TRANFERE CANETAS PARA O COMPRADOR
    while (1) {
        // Aguardar canetas disponiveis
        sem_wait(&sem_canetas);

        // Armazenar canetas
        pthread_mutex_lock(&mutex);
        if(canetas_disponiveis<=qtde_maxima_canetas){
            // Adicionar canetas ao deposito
            canetas_disponiveis++;
            printf("Caneta armazenada: %d\n", canetas_disponiveis);
            sem_post(&sem_canetas);

        }
        else {
            printf("Deposito de canetas cheio\n");
            printf("Canetas disponiveis: %d\n", canetas_disponiveis);
        }
        pthread_cond_broadcast(&cond_canetas); // Notificar possiveis compradores
        sleep(tempo_fabricar_caneta);//demora para fabricar uma caneta

//TODO Caso o deposito fique cheio, acordar o comprador, passar a quantidade de canetas para ele e colocar na thread a quantidade restante de canetas e deixar essa nova solicita��o, at� que ele consiga todas as canetas
void *comprador(void *args) {//rank 5
    while (1) {
        // Comprar canetas
        sleep(tempo_espera_compra);

        // Retirar canetas do deposito
        pthread_mutex_lock(&mutex);

        if (canetas_disponiveis > 0) {
            if (canetas_disponiveis >= qtde_comprada_interacao) {
                // Compra possível, retirar as canetas do deposito
                for (int i = 0; i < qtde_comprada_interacao; i++) {
                   canetas_disponiveis--;
                    printf("Comprador comprou uma caneta. Canetas restantes: %d\n", canetas_disponiveis);
                }
                printf("Comprador comprou todas as canetas REQUISITADAS\n");
            }else if(canetas_disponiveis < qtde_comprada_interacao){
                for (int i = 0; i < canetas_disponiveis; i++) {
                    canetas_disponiveis--;
                    printf("Comprador comprou uma caneta. Canetas restantes: %d\n", canetas_disponiveis);
                }
                printf("Comprador comprou todas as canetas DISPONIVEIS\n");
            }
        }else {
            // Nao ha canetas suficientes disponiveis
            printf("Comprador: Não há canetas suficientes disponiveis.\n");
        }
    pthread_mutex_unlock(&mutex);

    }
}
