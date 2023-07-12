#!/bin/bash

# OBS: lembrar de dar permissao de execucao:
# chmod +x script_execucao.sh

if [ $# -ne 1 ];
then
    echo "Erro: informe o numero de processos"
fi

# numero de processos
NUM_PROC=$1

NUM_RUN=10

for i in $(seq 1 $NUM_RUN)
do
    echo -n $NUM_PROC' '
    ./main_fifo $NUM_PROC | grep TME | sed 's/TME: //'
done

