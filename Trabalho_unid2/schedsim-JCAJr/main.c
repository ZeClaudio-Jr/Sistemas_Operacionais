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

int NPROC;                                          // Número de processos

int MAX_TIME = 100;                                 // Tempo máximo para a simulação

int QUANTUM = 20;                                   // Quantum de tempo para cada processo

double INTERRUPT_PROB = 0.5;                        // Probabilidade de ocorrência de uma interrupção

pthread_t sched_tid;                                // ID da thread de escalonamento

struct proc *running = NULL;                        // Ponteiro para o processo em execução

struct queue *ready;                                // Fila de prontos
struct queue *ready2;                               // Segunda fila de prontos
struct queue *blocked;                              // Fila de bloqueados
struct queue *finished;                             // Fila de processos finalizados

char gbuffer[100];                                  // Buffer global para imprimir mensagens de depuração

int event_num = 1;                                  // Contador para o número de eventos no sistema

sem_t sem_scheduling;                               // Semáforo usado para controlar a thread de escalonamento

int snum;                                           // Variável relacionada a sinais
sigset_t set;                                       // Conjunto de sinais que as threads de processo irão tratar

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Uso: %s [número de processos]\n", argv[0]);
        exit(1);
    }

    NPROC = atoi(argv[1]);

    if (NPROC <= 0)
    {
        printf("Por favor, forneça um valor válido [num > 0]\n");
        exit(2);
    }

    sem_init(&sem_scheduling, 0, 0);                // Inicializa o semáforo de escalonamento com valor 0

    sigemptyset(&set);
    if (sigaddset(&set, SIGUSR1) == -1)             // Adiciona o sinal SIGUSR1 ao conjunto de sinais
    {
        perror("Erro ao adicionar SIGUSR1 ao conjunto de sinais");
        pthread_exit((void *)1);
    }
    if (sigaddset(&set, SIGUSR2) == -1)             // Adiciona o sinal SIGUSR2 ao conjunto de sinais
    {
        perror("Erro ao adicionar SIGUSR2 ao conjunto de sinais");
        pthread_exit((void *)1);
    }

    srand(time(NULL));                              // Inicializa a semente do gerador de números aleatórios com o tempo atual

    printf("%s MAIN - Inicializando filas\n", event());

    ready = initqueue(ready);                       // Inicializa a fila de prontos
    ready2 = initqueue(ready2);                     // Inicializa a segunda fila de prontos
    blocked = initqueue(blocked);                   // Inicializa a fila de bloqueados
    finished = initqueue(finished);                 // Inicializa a fila de processos finalizados

    printf("%s MAIN - Inicializando os processos\n", event());

    proc_init();                                    // Inicializa os processos e os adiciona às filas apropriadas

    printf(">> MAIN - Imprimindo a fila 'ready':\n");
    printqueue(ready);

    printf(">> MAIN - Imprimindo a fila 'ready2':\n");
    printqueue(ready2);

    printf("%s MAIN - Iniciando o escalonador\n", event());

    start_scheduler();                              // Inicia o escalonador

    procend(finished);                              // Finaliza os processos e libera a memória alocada

    sem_destroy(&sem_scheduling);                    // Destrói o semáforo

    return 0;
}
