#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

// O problema do Buffer Limitado envolve a sincronização entre produtores e consumidores que compartilham um buffer de tamanho limitado. Os produtores colocam dados no buffer e os consumidores retiram os dados do buffer. Se o buffer estiver cheio, os produtores devem esperar até que haja espaço livre suficiente no buffer. Se o buffer estiver vazio, os consumidores devem esperar até que haja dados disponíveis no buffer.

//Os semáforos POSIX mutex, empty e full são usados para controlar o acesso ao buffer e garantir a sincronização correta entre as threads. São declaradas as variáveis globais necessárias, como sem_t mutex, sem_t empty, sem_t full para os semáforos, além de buffer para representar o buffer compartilhado, N_BUFFER para o tamanho do buffer e PROD_NUM para o número de produtores.
//  TO DO: Definição dos semáforos (variaveis precisam ser globais).  
sem_t mutex, empty, full; // Semáforos para exclusão mútua e controle de itens vazios e cheios


// ponteiro para a fila do buffer
int * buffer;

// buffer size
int N_BUFFER;

// producers num
int PROD_NUM;

// posicao do buffer a gravar
int p = 0;

// posicoes para acessar o buffer
int in = 0;

// Posição de saída no buffer
int out = 0;

// prototipos das funções
void *producer(void *);
void *consumer();
int gera_rand(int);
void print_buffer();


int main(int argc, char ** argv) //A função main() é o ponto de entrada do programa. Primeiro, são tratados os argumentos da linha de comando para obter o tamanho do buffer e o número de produtores. Em seguida, são inicializados os semáforos e o buffer. As threads produtoras são criadas usando o número de produtores fornecido. A thread consumidora também é criada.
{
    // thread do consumidor
    pthread_t tcons;

    // threads dos n produtores (vao ser alocadas dinamicamente)
    pthread_t *nprod;

    // int i;
    long i;

    if ( argc != 3 )
    {
        printf("Usage: %s buffer_size num_producers\n", argv[0]);
        return 0;
    }

    // iniciando a semente do random
    srand(time(NULL));

    // buffer size
    N_BUFFER = atoi(argv[1]);

    // num of producers
    PROD_NUM = atoi(argv[2]);

    //
    // TO DO: Criação dos semáforos (aqui é quando define seus valores)
    //
    sem_init(&mutex, 0, 1);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, N_BUFFER);
    
    // gerando um buffer de N inteiros
    buffer = malloc(N_BUFFER * sizeof(int)); // Alocação do buffer

    // "zerando" o buffer com valores -1
    for (i = 0; i < N_BUFFER; i++)
    {
        buffer[i] = -1; // Inicialização do buffer com valores inválidos
    }

    // gerando uma lista de threads de produtores
    nprod = malloc(PROD_NUM * sizeof(pthread_t)); // Alocação das threads produtoras

    // iniciando a thread do consumidor
    pthread_create(&tcons, NULL, consumer, NULL); // Criação da thread consumidora

    // iniciando as threads dos produtores
    for (i = 0; i < PROD_NUM; i++)
    {
        pthread_create(&nprod[i], NULL, producer, (void *)i); // Criação das threads produtoras
    }
    
    // iniciando as threads dos produtores
    for (i = 0; i < PROD_NUM; i++)
    {
        pthread_join(nprod[i], NULL); // Espera as threads produtoras terminarem
    }

    // finalizando a thread do consumidor
    pthread_join(tcons, NULL); // Espera a thread consumidora terminar

    //
    // TO DO: Excluindo os semaforos. Destruição dos semáforos
    //
    sem_destroy(&mutex);
    sem_destroy(&full);
    sem_destroy(&empty);
    
    // liberando a memoria alocada
    free(buffer); // Liberação da memória alocada para o buffer.
    free(nprod); // Liberação da memória alocada para as threads produtoras.

    return 0;
}

void * consumer(void *arg) //A função consumer() é responsável pela lógica do consumidor. Ela aguarda um tempo aleatório, entra em um loop para consumir os itens produzidos e realiza a impressão do buffer em diferentes momentos.
{
    usleep(gera_rand(1000000));

    // o valor consumido será salvo nesta variável
    int produto;

    int i = 0;

    // consome todos os recursos dos n produtores e finaliza
    while (i != PROD_NUM)
    {
        printf("- Consumidor esperando por recurso!\n");

        print_buffer();

        //
        // TO DO: precisa bloquear ate que tenha algo a consumir
        //
        sem_wait(&full);// O consumidor aguarda até que o buffer esteja cheio.
       
        
        printf("- Consumidor entrou em ação!\n");

        print_buffer();

        //
        // TO DO: precisa garantir exclusão mutua ao acessar o buffer
        //
        sem_wait(&mutex);// Obtém acesso exclusivo ao buffer.
      

        printf("\t- Consumidor vai limpar posição %d\n", out);

        // acessando o buffer
        produto = buffer[out];
        printf("\t- Consumiu o valor: %d\n",produto);

        // verificando se o consumidor nao esta consumindo sujeira do buffer
        if (buffer[out] == -1)
        {
            printf("==== ALERTA DO CONSUMIDOR ====\n");
            printf("Posicao %d estava vazia\n",in);
            printf("==============================\n");
        }
        
        // limpando o buffer nesta posição
        buffer[out] = -1;
        out = (out + 1) % N_BUFFER;// Atualiza o buffer.

        //
        // TO DO: saindo da seção critica
        //
        sem_post(&mutex);
        
        //
        // TO DO: liberando o recurso
        //
        sem_post(&empty);

        i++;
    }
}

// recebe o Id de um produtor. A função producer() é responsável pela lógica do produtor. Ela aguarda um tempo aleatório, entra em um loop para produzir os itens e realiza a impressão do buffer em diferentes momentos.
void * producer(void * id) {// Essa função producer é executada pelas threads produtoras. Ela simula o comportamento dos produtores.
    usleep(gera_rand(1000000)); // Aguarda um tempo aleatório antes de começar a produzir.

    // recebendo od Id do produtor e convertendo para int
    // int i = (int)id;
    long i = (long)id;
    
    // valor a ser produzido
    int produto;

    printf("> Produtor %d esperando por recurso!\n",i);

    //
    // TO DO: precisa bloquear até que tenha posicao disponível no buffer
    //
    sem_wait(&empty);// Cada produtor aguarda até que haja uma posição vazia no buffer.

    printf("> Produtor %d entrou em ação!\n",i);

    // 
    // TO DO: precisa garantir o acesso exclusivo ao buffer
    //
    sem_wait(&mutex);// Obtém acesso exclusivo ao buffer.

    // numero aleatorio de 0 a 99
    produto = gera_rand(100); // Gera um valor aleatório para produzir

    // verificando se o produtor nao esta sobrescrevendo uma posicao.
    if (buffer[in] != -1) // Verifica se a posição in do buffer já está ocupada.
    {
        printf("==== ALERTA DO PRODUTOR %d ====\n", i);
        printf("Posicao %d ocupada com o valor %d\n",in,buffer[in]);
        printf("===============================\n");
    }

    // gravando no buffer
    printf("\t> Produtor %d vai gravar o valor %d na pos %d\n", i, produto, in);
    buffer[in] = produto;// Grava o valor produzido na posição in do buffer.
    
    // avancando a posicao do buffer
    //p++; 
    in = (in + 1) % N_BUFFER;// Atualiza o contador in.

    // 
    // TO DO: liberar o acesso ao buffer
    //
    sem_post(&mutex);// Libera o acesso ao buffer.
    
    //
    // TO DO: liberar para o consumidor acessar o buffer
    //
    sem_post(&full);// Sinaliza que uma posição do buffer está cheia.
}

int gera_rand(int limit) //A função gera_rand() simplesmente gera um número aleatório dentro de um limite fornecido, de 0 a limit-1.
{
    // 0 a (limit - 1)
    return rand() % limit;
}

void print_buffer() //A função print_buffer() é utilizada para imprimir o estado atual do buffer. Imprime o conteúdo do buffer na saída padrão.
{
    int i;
    printf("\t== BUFFER ==\n");
    for (i = 0; i < N_BUFFER; i++)
    {
        printf("\ti: %d | v: %d\n",i,buffer[i]);
    }
    printf("\n");
}
