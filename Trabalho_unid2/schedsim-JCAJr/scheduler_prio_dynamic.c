#include <stdio.h>
#include "queue.h"
#include "proc.h"
#include "stats.h"
#include "utils.h"

// Declaração das variáveis externas para as filas de processos
extern struct queue *ready;
extern struct queue *ready2;
extern struct queue *blocked;
extern struct queue *finished;

// Variável externa para o tempo máximo do processo
extern int MAX_TIME;

// Função de escalonamento de processos
struct proc *scheduler(struct proc *current)
{
    struct proc *selected;
    double probability;

    // Verifica o estado do processo atual
    if (current != NULL)
    {
        switch (current->state)
        {
        case READY:
            // Adiciona o processo atual na fila "ready2"
            enqueue(ready2, current);

            // Contabiliza o processo na estatística de processos prontos
            count_ready_in(current);
            break;
        case BLOCKED:
            // Adiciona o processo atual na fila "blocked"
            enqueue(blocked, current);

            // Contabiliza o processo na estatística de processos bloqueados
            count_blocked_in(current);
            break;
        case FINISHED:
            // Adiciona o processo atual na fila "finished"
            enqueue(finished, current);

            // Contabiliza o processo na estatística de processos finalizados
            count_finished_in(current);
            break;
        default:
            // Caso o estado do processo não seja reconhecido, exibe uma mensagem de erro
            printf("@@ ERRO no estado de saída do processo %d\n", current->pid);
        }
    }

    // Verifica se as filas "ready" e "ready2" estão vazias
    if (isempty(ready) && isempty(ready2))
    {
        // Caso estejam ambas vazias, retorna NULL para indicar que não há processos para executar
        return NULL;
    }

    // Gera uma probabilidade aleatória entre 0 e 100
    probability = make_rand(100);

    // Verifica a probabilidade e o estado das filas para selecionar o próximo processo
    if ((probability < 80 && !isempty(ready)) || isempty(ready2))
    {
        // Seleciona o próximo processo da fila "ready"
        selected = dequeue(ready);
    }
    else
    {
        // Seleciona o próximo processo da fila "ready2"
        selected = dequeue(ready2);
    }

    // Contabiliza a saída do processo selecionado na estatística de processos prontos
    count_ready_out(selected);

    // Define o estado do processo selecionado como "RUNNING"
    selected->state = RUNNING;

    // Retorna o processo selecionado
    return selected;
}