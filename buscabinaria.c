#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    long timestamp;
    char sensor[17];
    char valor[64];
} Leitura;

// Busca binária adaptada para vetor ordenado em ordem decrescente
int busca_binaria_decrescente(Leitura *v, int n, long target) {
    int low = 0, high = n - 1;

    if (target >= v[0].timestamp) return 0;
    if (target <= v[n - 1].timestamp) return n - 1;

    while (low <= high) {
        int mid = (low + high) / 2;
        if (v[mid].timestamp == target) return mid;

        if (v[mid].timestamp > target) {
            // Como está decrescente, se v[mid] > target, buscamos direita (maior índice)
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    // low > high, escolher o mais próximo entre low e high
    if (low >= n) low = n - 1;
    if (high < 0) high = 0;

    long diffLow = labs(v[low].timestamp - target);
    long diffHigh = labs(v[high].timestamp - target);

    return (diffLow < diffHigh) ? low : high;
}

int main(int argc, char *argv[]) {
    if (argc != 7) {
        fprintf(stderr, "Uso: %s SENSOR dd mm aaaa hh mm ss\n", argv[0]);
        return 1;
    }

    char *nome_sensor = argv[1];
    int dia = atoi(argv[2]);
    int mes = atoi(argv[3]);
    int ano = atoi(argv[4]);
    int hora = atoi(argv[5]);
    int min = atoi(argv[6]);
    int seg = atoi(argv[7]);

    // Validações básicas de data e hora
    if (dia < 1 || dia > 31 || mes < 1 || mes > 12 || ano < 1970 || hora < 0 || hora > 23 || min < 0 || min > 59 || seg < 0 || seg > 59) {
        fprintf(stderr, "Data ou hora fora do intervalo valido.\n");
        return 1;
    }

    char nome_arquivo[32];
    snprintf(nome_arquivo, sizeof(nome_arquivo), "%s.txt", nome_sensor);
    FILE *f = fopen(nome_arquivo, "r");
    if (!f) {
        fprintf(stderr, "Arquivo %s nao encontrado.\n", nome_arquivo);
        return 1;
    }

    Leitura leituras[10000];
    int total = 0;
    int linha = 0;
    while (!feof(f) && total < 10000) {
        linha++;
        char buffer[256];
        if (!fgets(buffer, sizeof(buffer), f)) break;

        long ts;
        char sensor[17];
        char valor[64];

        int campos = sscanf(buffer, "%ld %16s %63[^\n]", &ts, sensor, valor);
        if (campos != 3) {
            fprintf(stderr, "Linha %d invalida no arquivo %s\n", linha, nome_arquivo);
            continue;
        }
        leituras[total].timestamp = ts;
        strcpy(leituras[total].sensor, sensor);
        strcpy(leituras[total].valor, valor);
        total++;
    }
    fclose(f);

    if (total == 0) {
        fprintf(stderr, "Arquivo vazio ou sem leituras validas.\n");
        return 1;
    }

    struct tm t = {0};
    t.tm_year = ano - 1900;
    t.tm_mon = mes - 1;
    t.tm_mday = dia;
    t.tm_hour = hora;
    t.tm_min = min;
    t.tm_sec = seg;
    t.tm_isdst = -1;

    time_t timestamp_consulta = mktime(&t);
    if (timestamp_consulta == -1) {
        fprintf(stderr, "Data invalida\n");
        return 1;
    }

    int idx = busca_binaria_decrescente(leituras, total, timestamp_consulta);

    printf("Leitura mais aproximada: timestamp=%ld, sensor=%s, valor=%s\n",
           leituras[idx].timestamp, leituras[idx].sensor, leituras[idx].valor);

    return 0;
}
