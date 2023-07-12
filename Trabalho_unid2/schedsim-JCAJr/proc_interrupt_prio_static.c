#include <stdio.h>

#include "queue.h"
#include "proc.h"
#include "stats.h"

extern struct queue * ready;   // Declaração externa da fila aptos
extern struct queue * ready2;  // Declaração externa da fila aptos2

void proc_interrupt(struct proc * p)
{
    if (p->queue == 1) {
        enqueue(ready, p);  // Adiciona o processo p à fila aptos
    } else {
        enqueue(ready2, p); // Adiciona o processo p à fila aptos2
    }
    
    p->state = READY;  // Define o estado do processo como aptos

    count_ready_in(p);  // Função para contar ou calcular com base no processo p
}