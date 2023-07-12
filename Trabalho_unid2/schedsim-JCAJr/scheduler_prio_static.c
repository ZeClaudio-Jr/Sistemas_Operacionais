#include <stdio.h>
#include "queue.h"
#include "proc.h"
#include "stats.h"
#include "utils.h"

extern struct queue * ready;
extern struct queue * ready2;
extern struct queue * blocked;
extern struct queue * finished;

extern int MAX_TIME;

struct proc * scheduler(struct proc * current)
{
    // Variáveis locais
    struct proc *selected;
    double probability;

    // Verifica o estado atual do processo em execução
    if (current != NULL)
    {
        switch (current->state)
        {
            case READY:
                // Coloca o processo na fila de prontos correspondente
                if (current->queue == 1) {
                    enqueue(ready, current);
                } else {
                    enqueue(ready2, current);
                }

                // Atualiza as estatísticas de processos prontos
                count_ready_in(current);
                break;
            case BLOCKED:
                // Coloca o processo na fila de bloqueados
                enqueue(blocked, current);

                // Atualiza as estatísticas de processos bloqueados
                count_blocked_in(current);
                break;
            case FINISHED:
                // Coloca o processo na fila de finalizados
                enqueue(finished, current);

                // Atualiza as estatísticas de processos finalizados
                count_finished_in(current);
                break;
            default:
                printf("@@ ERRO no estado de saída do processo %d\n", current->pid);
        }
    }

    // Verifica se não há processos nas filas de prontos
    if (isempty(ready) && isempty(ready2))
    {
        return NULL;
    }

    // Gera uma probabilidade aleatória
    probability = make_rand(100);

    // Seleciona um processo a ser executado
    if (probability < 80 && !isempty(ready)) {
        selected = dequeue(ready);
    } else if (!isempty(ready2)) {
        selected = dequeue(ready2);
    }

    // Atualiza as estatísticas de processos prontos
    count_ready_out(selected);

    // Define o estado do processo selecionado como em execução
    selected->state = RUNNING;

    // Retorna o processo selecionado para execução
    return selected;
}