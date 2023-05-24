#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

// TO DO: Definição dos semáforos (variaveis precisam ser globais)
//
sem_t *chopstick; // Semáforos dos chopsticks. Declaração de uma variável ponteiro para semáforos chopstick.
int N_FILOS; // Número de filósofos. Uma variável inteira N_FILOS para o número de filósofos.

// Protótipos das funções utilizadas no programa.
void *filosofo(void *);
void pegar(int, int);
void liberar(int, int);
int gera_rand(int);

int main(int argc, char **argv)
{
    // Threads dos filósofos
    pthread_t *tids;// Declaração de uma variável ponteiro para as threads dos filósofos tids. 
    long i;// Variável inteira i.

    srand(time(NULL));// Em seguida, é utilizado srand para inicializar a semente do gerador de números aleatórios.

    if (argc < 2)// Verifica se o número de argumentos passados é menor que 2 (indicando que o número de filósofos não foi fornecido como argumento). Se for o caso, exibe uma mensagem de uso e retorna 0 (encerra o programa).
    {
        printf("Usage: %s num_filosofos\n", argv[0]);// Exibe uma mensagem de uso.
        return 0;// Encerra o programa.
    }

    // Número de filósofos
    N_FILOS = atoi(argv[1]); // Converte o número de filósofos fornecido como argumento de string para inteiro usando atoi. Em seguida, aloca memória para os arrays tids e chopstick com base no número de filósofos.

    // Alocando memória para as listas
    tids = malloc(N_FILOS * sizeof(pthread_t));// Em seguida, aloca memória para os arrays tids com base no número de filósofos.

    chopstick = malloc(N_FILOS * sizeof(sem_t));// Em seguida, aloca memória para os arrays chopstick com base no número de filósofos.

    // Inicializando os semáforos dos chopsticks
    for (i = 0; i < N_FILOS; i++)
    {
        sem_init(&chopstick[i], 0, 1); // Inicializa os semáforos chopstick utilizando sem_init. Cada semáforo é inicializado com valor 1 (chopstick livre).
    }

    // Iniciando as threads dos filósofos
    for (i = 0; i < N_FILOS; i++)
    {
        pthread_create(&tids[i], NULL, filosofo, (void *)i);// Cria as threads dos filósofos utilizando pthread_create.
    }

    // Aguardando as threads dos filósofos terminarem
    for (i = 0; i < N_FILOS; i++)
    {
        pthread_join(tids[i], NULL);// Aguarda todas as threads dos filósofos terminarem utilizando pthread_join.
    }

    // Excluindo os semáforos
    for (i = 0; i < N_FILOS; i++)
    {
        sem_destroy(&chopstick[i]);// Destroi os semáforos chopstick utilizando sem_destroy.
    }

    // Liberando a memória alocada
    // Libera a memória alocada para os arrays tids e chopstick e encerra o programa.
    free(tids);
    free(chopstick);

    return 0;// Encerra o programa.
}

void *filosofo(void *id)// Função filosofo que representa o comportamento de um filósofo.   
{
    // Convertendo o ID do filósofo para int
    long i = (long)id;// Converte o id recebido para long

    printf("\t> Filosofo %ld pensando\n", i);// Imprime uma mensagem indicando que o filósofo está pensando.

    usleep(gera_rand(1000000));// Em seguida, utiliza usleep para fazer o filósofo esperar por um tempo aleatório.

    // Ordem dos chopsticks depende do ID.
    int c1, c2;

    // OBS: alterando a ordem de pegar o chopstick para evitar deadlock
    // Define a ordem dos chopsticks que o filósofo deve pegar com base no seu id.
    if (i % 2 == 0) // com base no id do filosofo (par ou impar)
    {
        // Os filósofos com ids pares pegam primeiro o chopstick com o mesmo id e depois o chopstick seguinte.
        c1 = i;                 // esquerda
        c2 = (i + 1) % N_FILOS; // direita
    }
    else
    {
        // Os filósofos com ids ímpares fazem o contrário, pegando primeiro o chopstick seguinte e depois o chopstick com o mesmo id.
        c1 = (i + 1) % N_FILOS; // direita
        c2 = i;                 // esquerda
    }

    // Precisa garantir que mais de um filósofo não pegue o mesmo chopstick simultaneamente.
    // Chama a função pegar para pegar os chopsticks na ordem definida. 
    pegar(i, c1);
    pegar(i, c2);

    printf("\t> Filosofo %ld comendo\n", i);// Em seguida, imprime uma mensagem indicando que o filósofo está comendo.

    usleep(gera_rand(1000000));// Utiliza usleep para fazer o filósofo esperar por um tempo aleatório enquanto está comendo.

    // Precisa garantir que os filósofos liberem os chopsticks após usar.
    //Chama a função liberar para liberar os chopsticks na ordem inversa e, em seguida, encerra a thread do filósofo utilizando pthread_exit.
    liberar(i, c1);
    liberar(i, c2);

    pthread_exit(NULL);// Encerra a thread do filósofo utilizando pthread_exit.
}

void pegar(int i, int num)// Função pegar que representa o filósofo pegando um chopstick. 
{
    sem_wait(&chopstick[num]); // Tentar pegar o chopstick. Utiliza sem_wait para bloquear o chopstick correspondente utilizando o semáforo.
    printf("+ Filosofo %d pegou o chopstick[%d]\n", i, num);
}

void liberar(int i, int num)// Função liberar que representa o filósofo liberando um chopstick. 
{
    sem_post(&chopstick[num]); // Liberar o chopstick. Utiliza sem_post para liberar o chopstick correspondente utilizando o semáforo.
    printf("- Filosofo %d liberou o chopstick[%d]\n", i, num);
}

int gera_rand(int limit) // Função gera_rand que gera um número aleatório entre 0 e limit - 1 utilizando rand. 
{
    // 0 a (limit - 1)
    return rand() % limit;// Retorna o número gerado.
}
