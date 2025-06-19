#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINHA 256
#define MAX_SENSOR_NOME 50


typedef struct {
    long timestamp;
    char sensor[MAX_SENSOR_NOME];
    char valor[100];
} Leitura;
Leitura* carregar_arquivo(const char *nomeArquivo, int *qtd) {
    FILE *f = fopen(nomeArquivo, "r");
    if (!f) {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        return NULL;
    }
    Leitura *leituras = NULL;
    int capacidade = 100;
    *qtd = 0;
    leituras = malloc(sizeof(Leitura) * capacidade);
    if (!leituras) {
        fclose(f);
        printf("Erro de memória.\n");
        return NULL;
    }
    char linha[MAX_LINHA];
    while (fgets(linha, sizeof(linha), f)) {
        if (*qtd == capacidade) {
            capacidade *= 2;
            Leitura *tmp = realloc(leituras, sizeof(Leitura) * capacidade);
            if (!tmp) {
                free(leituras);
                fclose(f);
                printf("Erro de memória.\n");
                return NULL;
            }
            leituras = tmp;
        }

        long ts;
        char sensor[MAX_SENSOR_NOME];
        char valor[100];

        int campos = sscanf(linha, "%ld %s %s", &ts, sensor, valor);
        if (campos != 3) continue;

        leituras[*qtd].timestamp = ts;
        strcpy(leituras[*qtd].sensor, sensor);
        strcpy(leituras[*qtd].valor, valor);
        (*qtd)++;
    }
    fclose(f);
    return leituras;
}



int busca_binaria_decrescente(Leitura *v, int n, long ts_alvo) {
    int low = 0;
    int high = n -1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (v[mid].timestamp == ts_alvo) {
            return mid;
        }
        if (v[mid].timestamp > ts_alvo) {
            low = mid + 1;
        } else {
            high = mid -1;
        }
    }



    if (low >= n) low = n-1;
    if (high < 0) high = 0;



    long diffLow = labs(v[low].timestamp - ts_alvo);
    long diffHigh = labs(v[high].timestamp - ts_alvo);
    return (diffLow < diffHigh) ? low : high;
}




int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <sensor> <timestamp>\n", argv[0]);
        return 1;
    }
    char nomeArquivo[100];
    snprintf(nomeArquivo, sizeof(nomeArquivo), "%s.txt", argv[1]);
    long ts_alvo = atol(argv[2]);
    int qtd;



    Leitura *leituras = carregar_arquivo(nomeArquivo, &qtd);
    if (!leituras) return 1;
    if (qtd == 0) {
        printf("Arquivo %s esta vazio.\n", nomeArquivo);
        free(leituras);
        return 1;
    }

    int idx = busca_binaria_decrescente(leituras, qtd, ts_alvo);


    printf("Letura mais proxima para timestamp %ld:\n", ts_alvo);
    printf("%ld %s %s\n", leituras[idx].timestamp, leituras[idx].sensor, leituras[idx].valor);
    free(leituras);
    return 0;
}
