#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

sem_t mutex, empty, full; // Semáforos para exclusão mútua e controle de itens vazios e cheios
int *buffer; // Ponteiro para o buffer
int N_BUFFER; // Tamanho do buffer
int PROD_NUM; // Número de produtores
int p = 0; // Posição para gravar no buffer
int in = 0; // Posição de entrada no buffer
int out = 0; // Posição de saída no buffer

void *producer(void *);
void *consumer(void *);
int gera_rand(int);
void print_buffer();

int main(int argc, char **argv)
{
    pthread_t tcons; // Thread do consumidor
    pthread_t *nprod; // Threads dos n produtores

    long i;

    if (argc != 3)
    {
        printf("Usage: %s buffer_size num_producers\n", argv[0]);
        return 0;
    }

    srand(time(NULL));

    N_BUFFER = atoi(argv[1]);
    PROD_NUM = atoi(argv[2]);

    sem_init(&mutex, 0, 1);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, N_BUFFER);

    buffer = malloc(N_BUFFER * sizeof(int));

    for (i = 0; i < N_BUFFER; i++)
    {
        buffer[i] = -1;
    }

    nprod = malloc(PROD_NUM * sizeof(pthread_t));

    pthread_create(&tcons, NULL, consumer, NULL);

    for (i = 0; i < PROD_NUM; i++)
    {
        pthread_create(&nprod[i], NULL, producer, (void *)i);
    }

    for (i = 0; i < PROD_NUM; i++)
    {
        pthread_join(nprod[i], NULL);
    }

    pthread_join(tcons, NULL);

    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    free(buffer);
    free(nprod);

    return 0;
}

void *consumer(void *arg)
{
    usleep(gera_rand(1000000));

    int produto;
    int i = 0;

    while (i != PROD_NUM)
    {
        printf("- Consumidor esperando por recurso!\n");
        print_buffer();

        sem_wait(&full); // Bloqueia se o buffer estiver vazio
        sem_wait(&mutex); // Bloqueia para acessar o buffer

        printf("- Consumidor entrou em ação!\n");
        print_buffer();





        //sem_wait(&mutex);
        //sem_wait(&empty);



        printf("\t- Consumidor vai limpar posição %d\n", out);

        produto = buffer[out];
        printf("\t- Consumiu o valor: %d\n", produto);

        if (buffer[out] == -1)
        {
            printf("==== ALERTA DO CONSUMIDOR ====\n");
            printf("Posicao %d estava vazia\n", in);
            printf("==============================\n");
        }

        buffer[out] = -1;
        out = (out + 1) % N_BUFFER;

        sem_post(&mutex); // Libera o acesso ao buffer
        sem_post(&empty); // Libera uma posição vazia no buffer

        i++;
    }
}

void *producer(void *id)
{
    usleep(gera_rand(1000000));

    long i = (long)id;
    int produto;

    printf("> Produtor %ld esperando por recurso!\n", i);

    sem_wait(&empty); // Bloqueia se o buffer estiver cheio
    sem_wait(&mutex); // Bloqueia para acessar o buffer

    printf("> Produtor %ld entrou em ação!\n", i);

    produto = gera_rand(100);

    if (buffer[in] != -1)
    {
        printf("==== ALERTA DO PRODUTOR %ld ====\n", i);
        printf("Posicao %d ocupada com o valor %d\n", in, buffer[in]);
        printf("===============================\n");
    }

    printf("\t> Produtor %ld vai gravar o valor %d na pos %d\n", i, produto, in);
    buffer[in] = produto;

    in = (in + 1) % N_BUFFER;

    sem_post(&mutex); // Libera o acesso ao buffer
    sem_post(&full); // Libera um item cheio no buffer
}

int gera_rand(int limit)
{
    return rand() % limit;
}

void print_buffer()
{
    int i;
    printf("\t== BUFFER ==\n");
    for (i = 0; i < N_BUFFER; i++)
    {
        printf("\ti: %d | v: %d\n", i, buffer[i]);
    }
    printf("\n");
}
