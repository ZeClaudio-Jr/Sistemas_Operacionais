#!/bin/bash

# Função para exibir a mensagem de uso
mostrar_mensagem_uso() {
    echo "Uso:"
    echo "  $0 [algoritmo] [num_processos] [valor_capturado]"
    echo ""
    echo "  Executa o [algorito] 100 vezes com [num_processos] processos e salva os tempos de execução do tipo [valor_capturado] em um arquivo de texto"
    echo ""
    echo "Parâmetros:"
    echo "  algoritmo: fifo | ljf | prio_dynamic | prio_static | sjf"
    echo "  num_processos: numero inteiro positivo"
    echo "  valor_capturado: TME | TMB | TMT"
}

# Verificar se o número de parâmetros é zero ou se foi passada a opção de ajuda
if [ $# -lt 3 ] || [ "$1" == "-h" ] || [ "$1" == "--help" ]; then
    mostrar_mensagem_uso
    exit 0
fi

# Nome do arquivo C
arquivo_c="main_$1"

# Número de processos
num_processos=$2

# valor a ser capturado
valor=$3

# Nome da pasta e do arquivo de saída
pasta_saida="dados_execucao/$1"
arquivo_saida="$3_$2.txt"

# Loop para executar o arquivo C 10 vezes
for ((i=1; i<= 100; i++))
do
    # Executa o arquivo C e redireciona a saída para o arquivo de saída
    mkdir -p "$pasta_saida" &&
    ./"$arquivo_c" $num_processos | grep -w "$valor" | grep -o "[0-9.]*" >> "./$pasta_saida/$arquivo_saida"
done