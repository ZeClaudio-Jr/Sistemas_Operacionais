#!/bin/bash

# Verifica se o arquivo foi dado como argumento
if [ $# -eq 0 ]; then
  echo "Uso: $0 arquivo"
  exit 1
fi

# Verifica se o tem o arquivo
if [ ! -f "$1" ]; then
  echo "O arquivo '$1' não existe."
  exit 1
fi

# Variável para armazenar a soma dos valores
soma=0
# Variável para contar o número de valores
contagem=0

# Loop para ler cada linha do arquivo
while IFS= read -r linha; do
  # Verifica se a linha contém um valor numérico
  if [[ $linha =~ ^[0-9]+([.][0-9]+)?$ ]]; then
    # Adiciona o valor à soma
    soma=$(awk "BEGIN {print $soma + $linha; exit}")
    # Incrementa a contagem
    contagem=$((contagem + 1))
  fi
done < "$1"

# Verifica se foram encontrados valores numéricos no arquivo
if [ $contagem -eq 0 ]; then
  echo "Nenhum valor numérico encontrado no arquivo."
  exit 1
fi

# Calcula a média
media=$(awk "BEGIN {printf \"%.3f\", $soma / $contagem; exit}")

# Mostra a média
echo "$media"