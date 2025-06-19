#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_LINHA 256
#define MAX_SENSORES 50
#define MAX_SENSOR_NOME 50

typedef struct {
    long timestamp;
    char sensor[MAX_SENSOR_NOME];
    char valor[100];
} Leitura;
typedef struct {
    char nome[MAX_SENSOR_NOME];
    Leitura *leituras;
    int qtd;
    int capacidade;
} Sensor;
int cmp_decrescente(const void *a, const void *b) {
    Leitura *la = (Leitura*)a;
    Leitura *lb = (Leitura*)b;
    if (la->timestamp > lb->timestamp) return -1;
    else if (la->timestamp < lb->timestamp) return 1;
    return 0;
}


int acha_ou_adiciona_sensor(Sensor sensores[], int *numSensores, const char *nome) {
    for (int i = 0; i < *numSensores; i++) {
        if (strcmp(sensores[i].nome, nome) == 0) return i;
    }
    if (*numSensores >= MAX_SENSORES) {
        printf("Limite maxiimo de sensores atingido.\n");
        exit(1);
    }
    strcpy(sensores[*numSensores].nome, nome);
    sensores[*numSensores].qtd = 0;
    sensores[*numSensores].capacidade = 100;
    sensores[*numSensores].leituras = malloc(sizeof(Leitura) * sensores[*numSensores].capacidade);
    if (!sensores[*numSensores].leituras) {
        printf("Erro de memoria.\n");
        exit(1);
    }
    (*numSensores)++;
    return (*numSensores) - 1;
}



int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <arquivo_entrada>\n", argv[0]);
        return 1;
    }
    FILE *f = fopen(argv[1], "r");
    if (!f) {
        printf("Erroo a abrir o arquivo %s\n", argv[1]);
        return 1;
    }
    Sensor sensores[MAX_SENSORES];
    int numSensores = 0;
    char linha[MAX_LINHA];
    int num_linha = 0;
    int linhas_invalidas = 0;


    while (fgets(linha, sizeof(linha), f)) {
        num_linha++;
        long timestamp;
        char sensor_nome[MAX_SENSOR_NOME];
        char valor[100];


        int campos = sscanf(linha, "%ld %s %s", &timestamp, sensor_nome, valor);
        if (campos != 3) {
            printf("Linha %d invalida: formato incorreto\n", num_linha);
            linhas_invalidas++;
            continue;
        }

        
        int idx = acha_ou_adiciona_sensor(sensores, &numSensores, sensor_nome);
        if (sensores[idx].qtd == sensores[idx].capacidade) {
            sensores[idx].capacidade *= 2;
            sensores[idx].leituras = realloc(sensores[idx].leituras, sizeof(Leitura) * sensores[idx].capacidade);
            if (!sensores[idx].leituras) {
                printf("Erro de memoria a o realocar.\n");
                fclose(f);
                return 1;
            }
        }
        sensores[idx].leituras[sensores[idx].qtd].timestamp = timestamp;
        strcpy(sensores[idx].leituras[sensores[idx].qtd].sensor, sensor_nome);
        strcpy(sensores[idx].leituras[sensores[idx].qtd].valor, valor);
        sensores[idx].qtd++;
    }

    fclose(f);




    if (linhas_invalidas > 0) {
        printf("Total de linhas invalidas: %d\n", linhas_invalidas);
    }
    for (int i = 0; i < numSensores; i++) {
        qsort(sensores[i].leituras, sensores[i].qtd, sizeof(Leitura), cmp_decrescente);
        char nome_arquivo[100];
        snprintf(nome_arquivo, sizeof(nome_arquivo), "%s.txt", sensores[i].nome);
        FILE *fs = fopen(nome_arquivo, "w");
        if (!fs) {
            printf("Erro ao criar arquivo %s\n", nome_arquivo);
            continue;
        }
        for (int j = 0; j < sensores[i].qtd; j++) {
            fprintf(fs, "%ld %s %s\n",
                    sensores[i].leituras[j].timestamp,
                    sensores[i].leituras[j].sensor,
                    sensores[i].leituras[j].valor);
        }
        fclose(fs);


        free(sensores[i].leituras);
        printf("Arquivo %s gerado com %d leituras.\n", nome_arquivo, sensores[i].qtd);
    }



    return 0;

}
