#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define MAX_SENSORES 10
#define MAX_NOME_SENSOR 20
#define MAX_TEXTO 16

int rand_int(int min, int max) {
    return min + rand() % (max - min + 1);
}
float rand_float(float min, float max) {
    return min + ((float)rand() / RAND_MAX) * (max - min);
}



void rand_text(char *str, int tamanho) {
    static const char letras[] = "abcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < tamanho - 1; i++) {
        str[i] = letras[rand() % (sizeof(letras) - 1)];
    }
    str[tamanho - 1] = '\0';
}
time_t converter_para_timestamp(int dia, int mes, int ano, int hora, int min, int seg) {
    struct tm t = {0};
    t.tm_year = ano - 1900;
    t.tm_mon = mes - 1;
    t.tm_mday = dia;
    t.tm_hour = hora;
    t.tm_min = min;
    t.tm_sec = seg;
    t.tm_isdst = -1;
    return mktime(&t);
}

time_t gerar_timestamp_aleatorio(time_t inicio, time_t fim) {
    return inicio + rand() % (fim - inicio + 1);
}





int main(int argc, char *argv[]) {
    if (argc < 5 || (argc - 3) % 2 != 0) {
        printf("Uso: %s <Dia_inicio> <mes_inicio> <ano_inicio> <hora_inicio> <min_inicio> <seg_inicio> <dia_fim> <mes_fim> <ano_fim> <hora_fim> <min_fim> <seg_fim> <sensor1> <tipo1> [<sensor2> <tipo2> ...]\n", argv[0]);
        printf("Tipos: CONJ_Z, CONJ_Q, TEXTO, BINA\n");
        return 1;
    }


    int di = atoi(argv[1]);
    int mi = atoi(argv[2]);
    int ai = atoi(argv[3]);
    int hi = atoi(argv[4]);
    int mii = atoi(argv[5]);
    int si = atoi(argv[6]);
    int df = atoi(argv[7]);
    int mf = atoi(argv[8]);
    int af = atoi(argv[9]);
    int hf = atoi(argv[10]);
    int mfim = atoi(argv[11]);
    int sf = atoi(argv[12]);



    time_t inicio = converter_para_timestamp(di, mi, ai, hi, mii, si);
    time_t fim = converter_para_timestamp(df, mf, af, hf, mfim, sf);
    if (fim < inicio) {
        printf("Data final menor que data inicial.\n");
        return 1;
    }



    int qtdSensores = (argc - 13) / 2;
    if (qtdSensores > MAX_SENSORES) {
        printf("Numero maxmo de sensores ultrapassado (%d).\n", MAX_SENSORES);
        return 1;
    }
    char sensores[MAX_SENSORES][MAX_NOME_SENSOR];
    char tipos[MAX_SENSORES][10];

    for (int i = 0; i < qtdSensores; i++) {
        strncpy(sensores[i], argv[13 + i * 2], MAX_NOME_SENSOR -1);
        sensores[i][MAX_NOME_SENSOR -1] = '\0';
        strncpy(tipos[i], argv[14 + i * 2], 9);
        tipos[i][9] = '\0';

        if (strcmp(tipos[i], "CONJ_Z") != 0 && strcmp(tipos[i], "CONJ_Q") != 0 &&
            strcmp(tipos[i], "TEXTO") != 0 && strcmp(tipos[i], "BINARIO") != 0) {
            printf("Tipo invalido para sensor %s: %s\n", sensores[i], tipos[i]);
            return 1;
        }
    }
    FILE *f = fopen("arquivo_teste.txt", "w");
    if (!f) {
        printf("Erro ao criar arquivo de saiiada.\n");
        return 1;
    }
    srand((unsigned int)time(NULL));


    for (int s = 0; s < qtdSensores; s++) {
        for (int i = 0; i < 2000; i++) {
            time_t ts = gerar_timestamp_aleatorio(inicio, fim);

    

            fprintf(f, "%ld %s ", ts, sensores[s]);
            if (strcmp(tipos[s], "CONJ_Z") == 0) {
                int v = rand_int(-1000, 1000);
                fprintf(f, "%d\n", v);
            } else if (strcmp(tipos[s], "CONJ_Q") == 0) {
                float v = rand_float(-1000.0f, 1000.0f);
                fprintf(f, "%.4f\n", v);
            } else if (strcmp(tipos[s], "TEXTO") == 0) {
                char texto[MAX_TEXTO];
                int tam = rand_int(5, MAX_TEXTO-1);
                rand_text(texto, tam);
                fprintf(f, "%s\n", texto);
            } else if (strcmp(tipos[s], "BINARIO") == 0) {
                int b = rand() % 2;
                fprintf(f, "%s\n", b ? "true" : "false");
            }
        }
    }
    fclose(f);
    printf("Arquivo arquivo_teste.txt criaado com sucesso!\n");



    return 0;


}
