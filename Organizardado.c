#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_LEITURAS 10000
#define MAX_SENSORES 100

typedef struct {
    long timestamp;
    char sensor[17];  // até 16 caracteres + '\0'
    char valor[64];   // para armazenar valores como string inicialmente
} Leitura;

int cmp_timestamp_decrescente(const void *a, const void *b) {
    Leitura *la = (Leitura *)a;
    Leitura *lb = (Leitura *)b;
    if (la->timestamp < lb->timestamp) return 1;
    else if (la->timestamp > lb->timestamp) return -1;
    else return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s arquivo_entrada\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        fprintf(stderr, "Erro ao abrir arquivo %s: %s\n", argv[1], strerror(errno));
        return 1;
    }

    Leitura leituras[MAX_LEITURAS];
    int total = 0;
    int linha = 0;

    // Ler arquivo e validar linha por linha
    while (!feof(f) && total < MAX_LEITURAS) {
        linha++;
        char buffer[256];
        if (!fgets(buffer, sizeof(buffer), f)) break;

        long ts;
        char sensor[17], valor[64];
        int campos = sscanf(buffer, "%ld %16s %63[^\n]", &ts, sensor, valor);

        if (campos != 3) {
            fprintf(stderr, "Linha %d invalida: formato incorreto\n", linha);
            continue; // ignora linha inválida
        }

        // Armazenar leitura
        leituras[total].timestamp = ts;
        strcpy(leituras[total].sensor, sensor);
        strcpy(leituras[total].valor, valor);
        total++;
    }
    fclose(f);

    if (total == 0) {
        fprintf(stderr, "Nenhuma leitura valida encontrada.\n");
        return 1;
    }

    // Encontrar sensores únicos
    char sensores_unicos[MAX_SENSORES][17];
    int num_sensores = 0;

    for (int i = 0; i < total; i++) {
        int achou = 0;
        for (int j = 0; j < num_sensores; j++) {
            if (strcmp(leituras[i].sensor, sensores_unicos[j]) == 0) {
                achou = 1;
                break;
            }
        }
        if (!achou) {
            if (num_sensores >= MAX_SENSORES) {
                fprintf(stderr, "Numero maximo de sensores excedido.\n");
                break;
            }
            strcpy(sensores_unicos[num_sensores++], leituras[i].sensor);
        }
    }

    // Para cada sensor, filtra e ordena decrescente
    for (int s = 0; s < num_sensores; s++) {
        Leitura leituras_sensor[MAX_LEITURAS];
        int count_sensor = 0;
        for (int i = 0; i < total; i++) {
            if (strcmp(leituras[i].sensor, sensores_unicos[s]) == 0) {
                leituras_sensor[count_sensor++] = leituras[i];
            }
        }

        qsort(leituras_sensor, count_sensor, sizeof(Leitura), cmp_timestamp_decrescente);

        char nome_arquivo[32];
        snprintf(nome_arquivo, sizeof(nome_arquivo), "%s.txt", sensores_unicos[s]);
        FILE *fs = fopen(nome_arquivo, "w");
        if (!fs) {
            fprintf(stderr, "Erro ao criar arquivo %s\n", nome_arquivo);
            continue;
        }
        for (int i = 0; i < count_sensor; i++) {
            fprintf(fs, "%ld %s %s\n",
                leituras_sensor[i].timestamp,
                leituras_sensor[i].sensor,
                leituras_sensor[i].valor);
        }
        fclose(fs);
        printf("Arquivo %s criado com %d leituras.\n", nome_arquivo, count_sensor);
    }

    return 0;
}
