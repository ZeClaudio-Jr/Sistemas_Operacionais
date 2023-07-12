import pandas as pd
import matplotlib.pyplot as plt

#####
# Pre-processamento dos resultados do algoritmo FIFO
#####

# carregando o arquivo de resultados
data_fifo = pd.read_csv('fifo.txt', header=None, sep=' ')
data_sjf = pd.read_csv('sjf.txt', header=None, sep=' ')
data_ljf = pd.read_csv('ljf.txt', header=None, sep=' ')
data_prio_dynamic = pd.read_csv('prio_dynamic.txt', header=None, sep=' ')
data_prio_static = pd.read_csv('prio_static.txt', header=None, sep=' ')

# renomeando as colunas para ficar mais facil de analisar
data_fifo.rename(columns={0: 'num_proc', 1: 'tempos'}, inplace=True)
data_sjf.rename(columns={0: 'num_proc', 1: 'tempos'}, inplace=True)
data_ljf.rename(columns={0: 'num_proc', 1: 'tempos'}, inplace=True)
data_prio_dynamic.rename(columns={0: 'num_proc', 1: 'tempos'}, inplace=True)
data_prio_static.rename(columns={0: 'num_proc', 1: 'tempos'}, inplace=True)

# convertendo todo os valores de tempo, de micro para milisegundos 
data_fifo['tempos'] = data_fifo['tempos']/1000
data_sjf['tempos'] = data_sjf['tempos']/1000
data_ljf['tempos'] = data_ljf['tempos']/1000
data_prio_dynamic['tempos'] = data_prio_dynamic['tempos']/1000
data_prio_static['tempos'] = data_prio_static['tempos']/1000

# computando media e variancia da coluna dos 'tempos', agrupando pelos
# valores da coluna num_proc (das quantidades de processos)
data_fifo2 = data_fifo.groupby(['num_proc'], as_index=False).agg({'tempos': ['mean', 'var']})
data_sjf2 = data_sjf.groupby(['num_proc'], as_index=False).agg({'tempos': ['mean', 'var']})
data_ljf2 = data_ljf.groupby(['num_proc'], as_index=False).agg({'tempos': ['mean', 'var']})
data_prio_dynamic2 = data_prio_dynamic.groupby(['num_proc'], as_index=False).agg({'tempos': ['mean', 'var']})
data_prio_static2 = data_prio_static.groupby(['num_proc'], as_index=False).agg({'tempos': ['mean', 'var']})

# renomeando as colunas de data2
data_fifo2.columns = ['num_proc', 'tempos_mean', 'tempos_var']
data_sjf2.columns = ['num_proc', 'tempos_mean', 'tempos_var']
data_ljf2.columns = ['num_proc', 'tempos_mean', 'tempos_var']
data_prio_dynamic2.columns = ['num_proc', 'tempos_mean', 'tempos_var']
data_prio_static2.columns = ['num_proc', 'tempos_mean', 'tempos_var']

# adicionando uma coluna com o nome do algoritmo
data_fifo2['algoritmo'] = 'FIFO'
data_sjf2['algoritmo'] = 'SJF'
data_ljf2['algoritmo'] = 'LJF'
data_prio_dynamic2['algoritmo'] = 'PRIO DYNAMIC'
data_prio_static2['algoritmo'] = 'PRIO STATIC'

#####
# Pre-processamento dos resultados do algoritmo ...
#####

# FIX: aqui estou apenas simulando que há dados do SJF
# estou fazendo apenas uma copia do FIFO para ele

# data_sjf2 = data_fifo2.copy()
# data_sjf2['tempos_mean'] = data_sjf2['tempos_mean'] - 0.5
# data_sjf2['algoritmo'] = 'SJF'

# repita aqui o mesmo tratamento acima, mas para os demais algoritmos

#####
# Fazendo um merge dos dados de todos os algoritmos
#####
data_all = pd.concat([data_fifo2, data_sjf2, data_ljf2, data_prio_dynamic2, data_prio_static2])  # adicionar os demais

#####
# Plotando o gráfico para os algoritmos
#####
fig, ax = plt.subplots()
# os dados são agrupados pela coluna 'algoritmo'
for key, group in data_all.groupby('algoritmo'):
    group.plot(x='num_proc', y='tempos_mean', yerr='tempos_var',
               capsize=3, label=key, ax=ax)

# renomeando os eixos
plt.xlabel('Número de processos')
plt.ylabel('Tempos de execução (ms)')

# ajustando o eixo x para aparecer todos os pontos
plt.xticks(range(10, 101, 10))

# salvando o plot em um arquivo
plt.savefig('graficos.png', dpi=300)
