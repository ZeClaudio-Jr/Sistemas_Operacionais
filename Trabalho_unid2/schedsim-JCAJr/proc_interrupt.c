#include <stdio.h>

#include "queue.h"
#include "proc.h"
#include "stats.h"

extern struct queue * ready;    // fila de aptos
extern struct queue * ready2;   // segunda fila de aptos

void proc_interrupt(struct proc * p)
{
    enqueue(ready, p);  // Insere o processo na fila de aptos

    p->state = READY;   // Atualiza o estado do processo para "READY"

    count_ready_in(p);  // Atualiza as estatísticas relacionadas aos processos aptos
}
