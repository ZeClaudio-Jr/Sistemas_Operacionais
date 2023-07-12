#include <stdio.h>
#include "queue.h"
#include "proc.h"
#include "stats.h"

extern int NPROC;
extern struct queue *ready;
extern struct queue *ready2;
extern struct queue *blocked;
extern struct queue *finished;

void proc_init()
{
    struct proc *p;

    // Itera NPROC vezes para criar os processos
    for (int i = 0; i < NPROC; i++) 
    {
        p = createproc();  // Cria um novo processo

        p->tid = createproc_thread(p);  // Cria uma nova thread para o processo

        enqueue(ready, p);  // Enfileira o processo na fila 'ready'

        count_ready_in(p);  // Realiza contagem de processos aptos
    }
}
