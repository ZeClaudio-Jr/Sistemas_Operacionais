/*Essas linhas incluem os arquivos de cabeçalho necessários para o programa. A biblioteca <stdio.h> é necessária para as operações de entrada e saída, enquanto "queue.h", "proc.h", "stats.h" e "utils.h" contêm as definições de estruturas de dados e funções utilizadas no código.*/
#include <stdio.h>
#include "queue.h"
#include "proc.h"
#include "stats.h"
#include "utils.h"

/*Essas linhas declaram variáveis externas que são ponteiros para as estruturas de dados de filas (queues). As filas são usadas para armazenar os processos em diferentes estados, como "prontos" (ready), "bloqueados" (blocked) e "finalizados" (finished).*/
extern struct queue *ready;
extern struct queue *ready2;
extern struct queue *blocked;
extern struct queue *finished;

extern int MAX_TIME; //Declara uma variável externa MAX_TIME que representa o tempo máximo de execução.

/*Declaração da função scheduler que recebe um ponteiro para um processo current e retorna um ponteiro para um processo struct proc. Essa função é responsável por selecionar qual processo será executado em seguida com base em um algoritmo de agendamento.*/
struct proc *scheduler(struct proc *current)
{
    //Declaração de ponteiros para estruturas de processos selected e currentIteratedProcess.
    struct proc *selected;
    struct proc *currentIteratedProcess;

    // Verifica o estado do processo atual e o coloca na fila apropriada
    if (current != NULL)  //Verifica se o processo atual não é nulo.
    {
        switch (current->state)
        {
        //Se o estado do processo atual for "pronto" (READY), ele é adicionado à fila de processos prontos (ready) usando a função enqueue. Além disso, a função count_ready_in é chamada para atualizar estatísticas relacionadas aos processos prontos.
        case READY:
            enqueue(ready, current);
            count_ready_in(current);
            break;
        //Se o estado do processo atual for "bloqueado" (BLOCKED), ele é adicionado à fila de processos bloqueados (blocked) usando a função enqueue. A função count_blocked_in é chamada para atualizar estatísticas relacionadas aos processos bloqueados.
        case BLOCKED:
            enqueue(blocked, current);
            count_blocked_in(current);
            break;
        //Se o estado do processo atual for "finalizado" (FINISHED), ele é adicionado à fila de processos finalizados (finished) usando a função enqueue. A função count_finished_in é chamada para atualizar estatísticas relacionadas aos processos finalizados.
        case FINISHED:
            enqueue(finished, current);
            count_finished_in(current);
            break;
        default:
            printf("@@ ERRO no estado de saída do processo %d\n", current->pid);
        }
    }

    // Verifica se a fila de processos prontos está vazia
    if (isempty(ready))
    {
        return NULL;
    }

    // Seleciona o próximo processo a ser executado com base em um algoritmo de agendamento
    selected = ready->head;
    currentIteratedProcess = selected->next;

    while (currentIteratedProcess)
    {
        if (selected->remaining_time < currentIteratedProcess->remaining_time)
        {
            selected = currentIteratedProcess;
        }
        currentIteratedProcess = currentIteratedProcess->next;
    }

    // Remove o processo selecionado da fila de processos prontos
    dequeue_bypid(ready, selected->pid);

    // Atualiza as estatísticas
    count_ready_out(selected);

    // Define o estado do processo selecionado como "em execução"
    selected->state = RUNNING;

    return selected;
}
