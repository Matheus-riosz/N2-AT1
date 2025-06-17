#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_SENSORES 20
#define LEITURAS_POR_SENSOR 2000

typedef enum { TIPO_INTEIRO, TIPO_FLOAT, TIPO_STRING, TIPO_BOOLEANO, TIPO_INVALIDO } TipoSensor;

typedef struct {
    char nome[17];
    TipoSensor tipo;
} Sensor;

int validar_data(int d, int m, int a, int h, int min, int s) {
    if (a < 1970 || m < 1 || m > 12 || d < 1 || d > 31) return 0;
    if (h < 0 || h > 23 || min < 0 || min > 59 || s < 0 || s > 59) return 0;
    return 1;
}

time_t converter_para_timestamp(int d, int m, int a, int h, int min, int s) {
    struct tm t = {0};
    t.tm_year = a - 1900;
    t.tm_mon = m - 1;
    t.tm_mday = d;
    t.tm_hour = h;
    t.tm_min = min;
    t.tm_sec = s;
    t.tm_isdst = -1;
    time_t ts = mktime(&t);
    if (ts == -1) {
        fprintf(stderr, "Data invalida.\n");
        exit(1);
    }
    return ts;
}

time_t gerar_timestamp_aleatorio(time_t ini, time_t fim) {
    if (fim < ini) {
        fprintf(stderr, "Intervalo de datas invalido.\n");
        exit(1);
    }
    return ini + rand() % (fim - ini + 1);
}

TipoSensor tipo_str_para_enum(const char *tipo_str) {
    if (strcmp(tipo_str, "CONJ_Z") == 0) return TIPO_INTEIRO;
    if (strcmp(tipo_str, "CONJ_Q") == 0) return TIPO_FLOAT;
    if (strcmp(tipo_str, "TEXTO") == 0) return TIPO_STRING;
    if (strcmp(tipo_str, "BINARIO") == 0) return TIPO_BOOLEANO;
    return TIPO_INVALIDO;
}

char *gerar_valor_aleatorio(TipoSensor tipo, char *buffer, size_t bufsize) {
    switch (tipo) {
        case TIPO_INTEIRO:
            snprintf(buffer, bufsize, "%d", rand() % 1000);
            break;
        case TIPO_FLOAT:
            snprintf(buffer, bufsize, "%.2f", ((float)(rand() % 10000)) / 100.0);
            break;
        case TIPO_STRING:
            {
                // Gera string aleatória de até 16 letras
                int len = 5 + rand() % 11;
                for (int i = 0; i < len; i++) {
                    buffer[i] = 'A' + (rand() % 26);
                }
                buffer[len] = '\0';
            }
            break;
        case TIPO_BOOLEANO:
            strcpy(buffer, rand() % 2 ? "true" : "false");
            break;
        default:
            strcpy(buffer, "???");
    }
    return buffer;
}

int main(int argc, char *argv[]) {
    if (argc < 7) {
        fprintf(stderr, "Uso: %s dd_ini mm_ini aaaa_ini hh_ini mm_ini ss_ini dd_fim mm_fim aaaa_fim hh_fim mm_fim ss_fim nome_sensor tipo_sensor [...]\n", argv[0]);
        return 1;
    }

    // Para simplificar, pegamos as primeiras 6 args de inicio e as próximas 6 de fim
    if (argc < 14) {
        fprintf(stderr, "Numero insuficiente de argumentos para datas.\n");
        return 1;
    }

    // Validar datas de inicio
    int dd_ini = atoi(argv[1]);
    int mm_ini = atoi(argv[2]);
    int aaaa_ini = atoi(argv[3]);
    int hh_ini = atoi(argv[4]);
    int mi_ini = atoi(argv[5]);
    int ss_ini = atoi(argv[6]);

    if (!validar_data(dd_ini, mm_ini, aaaa_ini, hh_ini, mi_ini, ss_ini)) {
        fprintf(stderr, "Data inicial invalida.\n");
        return 1;
    }
    time_t ts_ini = converter_para_timestamp(dd_ini, mm_ini, aaaa_ini, hh_ini, mi_ini, ss_ini);

    // Validar datas de fim
    int dd_fim = atoi(argv[7]);
    int mm_fim = atoi(argv[8]);
    int aaaa_fim = atoi(argv[9]);
    int hh_fim = atoi(argv[10]);
    int mi_fim = atoi(argv[11]);
    int ss_fim = atoi(argv[12]);

    if (!validar_data(dd_fim, mm_fim, aaaa_fim, hh_fim, mi_fim, ss_fim)) {
        fprintf(stderr, "Data final invalida.\n");
        return 1;
    }
    time_t ts_fim = converter_para_timestamp(dd_fim, mm_fim, aaaa_fim, hh_fim, mi_fim, ss_fim);

    if (ts_fim <= ts_ini) {
        fprintf(stderr, "Data final deve ser maior que data inicial.\n");
        return 1;
    }

    // Restante argumentos são pares nome_sensor + tipo_sensor
    int num_sensores = (argc - 13) / 2;
    if (num_sensores <= 0) {
        fprintf(stderr, "Nenhum sensor informado.\n");
        return 1;
    }
    if (num_sensores > MAX_SENSORES) {
        fprintf(stderr, "Numero maximo de sensores excedido (%d).\n", MAX_SENSORES);
        return 1;
    }

    Sensor sensores[MAX_SENSORES];

    for (int i = 0; i < num_sensores; i++) {
        char *nome = argv[13 + 2 * i];
        char *tipo = argv[14 + 2 * i];

        if (strlen(nome) > 16) {
            fprintf(stderr, "Nome do sensor '%s' muito longo.\n", nome);
            return 1;
        }

        TipoSensor ts = tipo_str_para_enum(tipo);
        if (ts == TIPO_INVALIDO) {
            fprintf(stderr, "Tipo do sensor '%s' invalido. Tipos validos: CONJ_Z, CONJ_Q, TEXTO, BINARIO\n", tipo);
            return 1;
        }

        strcpy(sensores[i].nome, nome);
        sensores[i].tipo = ts;
    }

    FILE *out = fopen("arquivo_teste.txt", "w");
    if (!out) {
        fprintf(stderr, "Erro ao criar arquivo de teste.\n");
        return 1;
    }

    srand(time(NULL));

    for (int i = 0; i < num_sensores; i++) {
        for (int j = 0; j < LEITURAS_POR_SENSOR; j++) {
            time_t ts = gerar_timestamp_aleatorio(ts_ini, ts_fim);
            char valor[64];
            gerar_valor_aleatorio(sensores[i].tipo, valor, sizeof(valor));
            fprintf(out, "%ld %s %s\n", ts, sensores[i].nome, valor);
        }
    }

    fclose(out);
    printf("Arquivo de teste 'arquivo_teste.txt' criado com sucesso.\n");
    return 0;
}
