#include <stdio.h>

#include "queue.h" 
#include "proc.h" 
#include "stats.h" 
#include "utils.h" 

extern struct queue *ready;    
extern struct queue *ready2;  
extern struct queue *blocked;  
extern struct queue *finished; 
extern int MAX_TIME;

struct proc *scheduler(struct proc *current)
{
    // Ponteiros para processos selecionados
    struct proc *selected;
    struct proc *currentIteratedProcess;

    // Verifica o estado do processo atual
    if (current != NULL)
    {
        switch (current->state)
        {
        case READY:
            // Coloca o processo na fila de prontos
            enqueue(ready, current);

            // Atualiza estatísticas de entrada de prontos
            count_ready_in(current);
            break;
        case BLOCKED:
            // Coloca o processo na fila de bloqueados
            enqueue(blocked, current);

            // Atualiza estatísticas de entrada de bloqueados
            count_blocked_in(current);
            break;
        case FINISHED:
            // Coloca o processo na fila de finalizados
            enqueue(finished, current);

            // Atualiza estatísticas de entrada de finalizados
            count_finished_in(current);
            break;
        default:
            // Exibe mensagem de erro para estado de saída inválido
            printf("@@ ERRO no estado de saída do processo %d\n", current->pid);
        }
    }

    // Verifica se a fila de prontos está vazia
    if (isempty(ready))
    {
        return NULL;
    }

    // Seleciona o próximo processo a ser executado com base no menor tempo restante
    selected = ready->head;
    currentIteratedProcess = selected->next;

    while (currentIteratedProcess)
    {
        if (currentIteratedProcess->remaining_time < selected->remaining_time)
        {
            // Atualiza o processo selecionado com menor tempo restante
            selected = currentIteratedProcess;
        }
        currentIteratedProcess = currentIteratedProcess->next;
    }

    // Remove o processo selecionado da fila de prontos
    dequeue_bypid(ready, selected->pid);

    // Atualiza estatísticas de saída de prontos
    count_ready_out(selected);

    // Define o estado do processo selecionado como "EXECUTANDO"
    selected->state = RUNNING;

    // Retorna o processo selecionado para execução
    return selected;
}
