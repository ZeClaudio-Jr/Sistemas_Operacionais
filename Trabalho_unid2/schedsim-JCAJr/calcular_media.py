#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>

#include "queue.h"
#include "proc.h"
#include "proc_init.h"
#include "sched.h"
#include "utils.h"
#include "thread.h"

int NPROC;                                        // Número de processos a serem escalonados

int MAX_TIME = 100;                                 // Tempo máximo de execução para cada processo

int QUANTUM = 20;                                  // Quantum de tempo para o escalonador

double INTERRUPT_PROB = 0.5;                        // Probabilidade de ocorrer uma interrupção durante a execução de um processo

pthread_t sched_tid;                                // Identificador da thread do escalonador

struct proc *running = NULL;                        // Processo em execução no momento

struct queue *ready;                                // Fila de processos prontos
struct queue *ready2;                               // Fila de processos prontos para a segunda rodada de escalonamento
struct queue *blocked;                              // Fila de processos bloqueados
struct queue *finished;                             // Fila de processos finalizados

char gbuffer[100];                                  // Buffer para armazenar informações para impressão

int event_num = 1;                                  // Contador de eventos no sistema

sem_t sem_scheduling;                               // Semáforo para sincronização do escalonador com os processos

int snum;                                           // Número de sinais suportados pelo sistema operacional
sigset_t set;                                       // Conjunto de sinais

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Uso: %s [número de processos]\n", argv[0]);   // Exibe uma mensagem de uso se o número de argumentos não for igual a 2
        exit(1);
    }

    NPROC = atoi(argv[1]);                           // Converte o número de processos de string para inteiro

    if (NPROC <= 0)
    {
        printf("Por favor, forneça um valor válido [num > 0]\n");   // Verifica se o número de processos é válido
        exit(2);
    }

    sem_init(&sem_scheduling, 0, 0);                  // Inicializa o semáforo sem_scheduling com valor 0

    sigemptyset(&set);
    if (sigaddset(&set, SIGUSR1) == -1)                // Adiciona o sinal SIGUSR1 ao conjunto de sinais
    {
        perror("Erro ao adicionar SIGUSR1 ao conjunto de sinais");    // Exibe uma mensagem de erro se houver falha ao adicionar o sinal ao conjunto
        pthread_exit((void *)1);
    }
    if (sigaddset(&set, SIGUSR2) == -1)                // Adiciona o sinal SIGUSR2 ao conjunto de sinais
    {
        perror("Erro ao adicionar SIGUSR2 ao conjunto de sinais");    // Exibe uma mensagem de erro se houver falha ao adicionar o sinal ao conjunto
        pthread_exit((void *)1);
    }

    srand(time(NULL));                                // Inicializa o gerador de números aleatórios com o valor atual do relógio do sistema

    printf("%s MAIN - Inicializando filas\n", event());   // Imprime uma mensagem de inicialização das filas

    ready = initqueue(ready);                           // Inicializa a fila de processos prontos
    ready2 = initqueue(ready2);                         // Inicializa a fila de processos prontos para a segunda rodada de escalonamento
    blocked = initqueue(blocked);                       // Inicializa a fila de processos bloqueados
    finished = initqueue(finished);                     // Inicializa a fila de processos finalizados

    printf("%s MAIN - Inicializando os processos\n", event());   // Imprime uma mensagem de inicialização dos processos

    proc_init();                                        // Inicializa os processos

    printf(">> MAIN - Imprimindo a fila 'ready':\n");
    printqueue(ready);                                  // Imprime a fila 'ready'

    printf(">> MAIN - Imprimindo a fila 'ready2':\n");
    printqueue(ready2);                                 // Imprime a fila 'ready2'

    printf("%s MAIN - Iniciando o escalonador\n", event());   // Imprime uma mensagem de início do escalonador

    start_scheduler();                                  // Inicia o escalonador

    procend(finished);                                  // Finaliza os processos
    sem_destroy(&sem_scheduling);                       // Destrói o semáforo sem_scheduling

    return 0;                                           // Retorna 0 para indicar que a execução foi concluída com sucesso
}
