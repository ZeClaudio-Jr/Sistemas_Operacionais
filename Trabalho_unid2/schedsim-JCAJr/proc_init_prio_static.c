#include <stdio.h>

#include "queue.h"
#include "proc.h"
#include "stats.h"

extern int NPROC;                   // Número total de processos
extern int MAX_TIME;                // Tempo máximo

extern struct queue * ready;        // Fila de processos prontos
extern struct queue * ready2;       // Segunda fila de processos prontos
extern struct queue * blocked;      // Fila de processos bloqueados
extern struct queue * finished;     // Fila de processos finalizados

void proc_init()
{
    struct proc *p;                 // Ponteiro para a estrutura de processo

    int remaining_time_threshold = 0.2 * MAX_TIME; // Limite de tempo restante para determinar em qual fila adicionar o processo

    for (int i = 0; i < NPROC; i++) 
    {
        p = createproc();           // Cria um novo processo

        p->tid = createproc_thread(p);  // Cria uma nova thread para o processo e atribui o ID da thread

        if (p->remaining_time <= remaining_time_threshold) {
            p->queue = 1;           // Atribui a fila 1 para processos com tempo restante abaixo ou igual ao limite
            enqueue(ready, p);      // Adiciona o processo à fila de processos prontos (fila 1)
        } else {
            p->queue = 2;           // Atribui a fila 2 para processos com tempo restante acima do limite
            enqueue(ready2, p);     // Adiciona o processo à segunda fila de processos prontos (fila 2)
        }

        count_ready_in(p);           // Atualiza as estatísticas relacionadas aos processos prontos
    }
}