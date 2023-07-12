import os
import sys
from statistics import pvariance

# Obtém o nome da pasta a partir do primeiro argumento passado ao script
pasta = f'./dados_execucao/{sys.argv[1]}'

# Dicionário para armazenar os resultados calculados
resultado = {}

# Itera sobre os arquivos presentes na pasta
for nome_arquivo in os.listdir(pasta):
    # Obtém o caminho completo do arquivo
    caminho_arquivo = os.path.join(pasta, nome_arquivo)
    # Abre o arquivo para leitura
    with open(caminho_arquivo, 'r') as arquivo:
        # Lê cada linha do arquivo e converte para um número de ponto flutuante
        # Dividindo cada valor por 1000
        valores = [(float(linha) / 1000) for linha in arquivo]
        # Calcula a variância populacional dos valores e armazena no dicionário resultado
        resultado[nome_arquivo] = pvariance(valores)

# Imprime os resultados em ordem alfabética dos nomes dos arquivos
for key in sorted(resultado.keys()):
    print(key, '\t', resultado[key])
