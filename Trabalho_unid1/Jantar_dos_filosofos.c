#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

// TO DO: Definição dos semáforos (variaveis precisam ser globais)
//
sem_t *chopstick; // Semáforos dos chopsticks
int N_FILOS; // Número de filósofos

// Protótipos das funções
void *filosofo(void *);
void pegar(int, int);
void liberar(int, int);
int gera_rand(int);

int main(int argc, char **argv)
{
    // Threads dos filósofos
    pthread_t *tids;
    long i;

    srand(time(NULL));

    if (argc < 2)
    {
        printf("Usage: %s num_filosofos\n", argv[0]);
        return 0;
    }

    // Número de filósofos
    N_FILOS = atoi(argv[1]);

    // Alocando memória para as listas
    tids = malloc(N_FILOS * sizeof(pthread_t));
    chopstick = malloc(N_FILOS * sizeof(sem_t));

    // Inicializando os semáforos dos chopsticks
    for (i = 0; i < N_FILOS; i++)
    {
        sem_init(&chopstick[i], 0, 1); // Inicializa cada semáforo com 1 (chopstick livre)
    }

    // Iniciando as threads dos filósofos
    for (i = 0; i < N_FILOS; i++)
    {
        pthread_create(&tids[i], NULL, filosofo, (void *)i);
    }

    // Aguardando as threads dos filósofos terminarem
    for (i = 0; i < N_FILOS; i++)
    {
        pthread_join(tids[i], NULL);
    }

    // Excluindo os semáforos
    for (i = 0; i < N_FILOS; i++)
    {
        sem_destroy(&chopstick[i]);
    }

    // Liberando a memória alocada
    free(tids);
    free(chopstick);

    return 0;
}

void *filosofo(void *id)
{
    // Convertendo o ID do filósofo para int
    long i = (long)id;

    printf("\t> Filosofo %ld pensando\n", i);
    usleep(gera_rand(1000000));

    // Ordem dos chopsticks depende do ID
    int c1, c2;

    // OBS: alterando a ordem de pegar o chopstick para evitar deadlock
    if (i % 2 == 0) // com base no id do filosofo (par ou impar)
    {
        c1 = i;                 // esquerda
        c2 = (i + 1) % N_FILOS; // direita
    }
    else
    {
        c1 = (i + 1) % N_FILOS; // direita
        c2 = i;                 // esquerda
    }

    // Precisa garantir que mais de um filósofo não pegue o mesmo chopstick simultaneamente
    pegar(i, c1);
    pegar(i, c2);

    printf("\t> Filosofo %ld comendo\n", i);
    usleep(gera_rand(1000000));

    // Precisa garantir que os filósofos liberem os chopsticks após usar
    liberar(i, c1);
    liberar(i, c2);

    pthread_exit(NULL);
}

void pegar(int i, int num)
{
    sem_wait(&chopstick[num]); // Tentar pegar o chopstick
    printf("+ Filosofo %d pegou o chopstick[%d]\n", i, num);
}

void liberar(int i, int num)
{
    sem_post(&chopstick[num]); // Liberar o chopstick
    printf("- Filosofo %d liberou o chopstick[%d]\n", i, num);
}

int gera_rand(int limit)
{
    // 0 a (limit - 1)
    return rand() % limit;
}
