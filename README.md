
# Sistema de Organização e Consulta de Dados de Sensores Industriais


## Descrição do Projeto:
Este  projeto tem como objetivo deseenvolver um sistema para organização, geração e consulta de dados provenientes de sensores industriais. Em uma planta industrial inteligente, os sensores realizam medições constantes, e seus dados são armazenados em um arquivo.

O sistema desenvolvido é composto por três programas em linguagem C, que permitem:
- Organizar os dados dos sensores em arquivos separados, ordenados por timestamp em ordem decrescente.
- Realizar consultas rápidas e precisas através de busca binária sobre os dados organizados.
- Gerar arquivos de teste contendo leituras simuladas de sensores com diferentes tipos de dados, dentro de um intervalo de tempo especificado.

## Especificação dos Programas:
### PROG1_Org.c - Organização dos Dados
Este programa lê um arquivo contendo dados no formato:
<TIMESTAMP> <ID_SENSOR> <VALOR>

Ele processa este arquivo e executa as seguintes tarefas:
- Identifica os diferentes sensores presentes no arquivo.
- Separa os dados em arquivos distintos para cada sensor.
- Ordena os dados dentro de cada arquivo por timestamp, em ordem decrescente.
- Salva os dados organizados em arquivos com o nome do sensor seguido da extensão .txt.
Uso:
./PROG1_Org.exe <arquivo_entrada>
Exemplo:
.\PROG1_Org.exe arquivo_teste.txt

### PROG2_bus.c - Consulta por Timestamp
Este programa permite consultar a leitura de um sensor mais prooxima de um timestamp específico.

Funcionamento:
- Recebe como argumento o nome do sensor e o timestamp desejado.
- Acessa o arquivo correspondente ao sensor.
- Executa uma busca binária no arquivo (que deve estar ordenado em ordem decrescente).
- Retorna a leitura com timestamp mais próximo do informado.
Uso:

./PROG2_bus.exe <sensor> <timestamp>
Exemplo:
.\PROG2_bus.exe sensor1 1718662000

### PROG3_GER.c - Gerador de Arquivo de Teste
Este programa gera um arquivo contendo leituras simuladas de sensores.
Funcionamento:
- Recebe como entrada a data e hora inicial, a data e hora final e a lista de sensores com seus respectivos tipos de dados.
- Gera 2000 leituras para cada sensor, com valores aleatórios e timestamps dentro do intervalo informado.
- Salva o arquivo no formato:
<TIMESTAMP> <ID_SENSOR> <VALOR>

Tipos de dados suportados:
- CONJ_Z: valores inteiros.
- CONJ_Q: valores do tipo float.
- TEXTO: strings de até 16 caracteres.
- BINARIO: valores booleanos (true ou false).
Uso:

./PROG3_GER.exe <dia_inicio> <mes_inicio> <ano_inicio> <hora_inicio> <min_inicio> <seg_inicio> <dia_fim> <mes_fim> <ano_fim> <hora_fim> <min_fim> <seg_fim> <sensor1> <tipo1> [<sensor2> <tipo2> ...]
Exemplo de uso:
./PROG3_GER.exe 1 6 2025 0 0 0 30 6 2025 23 59 59 sensor1 CONJ_Z sensor2 TEXTO sensor3 BINARIO sensor4 CONJ_Q

## Validações Implementadas

- Verificação dos tipos de sensores permitidos (CONJ_Z, CONJ_Q, TEXTO, BINARIO).
- Validação dos argumentos de entrada, como datas inválidas ou tipos incorretos.
- Detecção de linhas mal formatadas nos arquivos de entrada.
- Mensagens claras no terminal informando erros e inconsistências.
- O programa não executa e não gera arquivos quando os argumentos estão fora do padrão esperado.

## Estrutura dos Arquivos

Cada linha dos arquivos gerados ou processados segue o formato:

<TIMESTAMP> <ID_SENSOR> <VALOR>

Exemplo:

1718662000 sensor1 245
1718661999 sensor1 512
1718661998 sensor1 -50

## Algoritmos Utilizados

- Ordenação dos dados utilizando o algoritmo qsort, configurado para ordem decrescente de timestamp.
- Busca binária adaptada para funcionamento com arquivos em ordem decrescente.

## Estrutura do Projeto

- PROG1_Org.c - Programa de organização dos dados.
- PROG2_bus.c - Programa de consulta por timestamp.
- PROG3_GER.c - Programa gerador de arquivo de teste.
- arquivo_teste.txt - Arquivo gerado contendo dados simulados.
- sensor1.txt, sensor2.txt, sensor3.txt, sensor4.txt - Arquivos organizados por sensor.
- README.md - Documento de especificação e instruções do projeto.

## Informações do Projeto

- Aluno: Matheus Rios
- Disciplina: Estrutura de Dados
- Professor: Zoe
- Instituição: Universidade Católica de Brasília (UCB