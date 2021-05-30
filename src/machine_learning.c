#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct matriz {
    int n_linhas;
    int n_colunas;
    float **data;
} Matriz;

typedef struct vetor {
    int n_elems;
    int *data;
} Vetor;

Matriz *cria_matriz(int n_linhas, int n_colunas) {
    Matriz *mat = (Matriz *) calloc(1, sizeof(Matriz));
    mat->n_linhas = n_linhas;
    mat->n_colunas = n_colunas;

    mat->data = (float **) calloc(mat->n_linhas, sizeof(float *));
    for (int i = 0; i < mat->n_linhas; i++) {
        mat->data[i] = (float *) calloc(mat->n_colunas, sizeof(float));
    }

    return mat;
}

void destroi_matriz(Matriz **mat_ref) {
    Matriz *mat = *mat_ref;

    for (int i = 0; i < mat->n_linhas; i++) {
        free(mat->data[i]);
    }
    free(mat->data);
    free(mat);
    *mat_ref = NULL;
}

Vetor *cria_vetor(int n_elems) {
    Vetor *vet = calloc(1, sizeof(Vetor));
    vet->n_elems = n_elems;
    vet->data = (int *) calloc(vet->n_elems, sizeof(int));

    return vet;
}

void destroi_vetor(Vetor **vet_ref) {
    Vetor *vet = *vet_ref;
    free(vet->data);
    free(vet);
    *vet_ref = NULL;
}

Vetor *knn(const Matriz *X_train, const Vetor *y_train, const Matriz *X_test, int k) {
    int n_amostras_teste = X_test->n_linhas;
    int n_amostras_treino = X_train->n_linhas;
    Vetor *y_test = cria_vetor(n_amostras_teste);

    for (int q = 0; q < n_amostras_teste; q++) {
        float *distancia = (float *) calloc(n_amostras_treino, sizeof(float));
        Vetor *indices = cria_vetor(n_amostras_treino);

        for (int p = 0; p < n_amostras_treino; p++) {
            distancia[p] = sqrt(pow(X_test->data[q][0] - X_train->data[p][0], 2) + pow(X_test->data[q][1] - X_train->data[p][1], 2));
        }

        float temp;
        int indice_temp;

        for (int i = 0; i < n_amostras_treino; i++) {
            indices->data[i] = i;
        }

        for (int i = 0; i < n_amostras_treino; i++) {
            for (int j = i + 1; j < n_amostras_treino; j++) {
                if (distancia[i] > distancia[j]) {
                    temp = distancia[i];
                    indice_temp = indices->data[i];

                    distancia[i] = distancia[j];
                    indices->data[i] = indices->data[j];

                    distancia[j] = temp;
                    indices->data[j] = indice_temp;
                }
            }
        }

        Vetor *rotulo = cria_vetor(k);
        for (int i = 0; i < k; i++) {
            rotulo->data[i] = y_train->data[indices->data[i]];
        }

        float soma = 0.0;
        for (int i = 0; i < k; i++) {
            soma += rotulo->data[i];
        }

        if ((soma /= k) > 0.5) {
            y_test->data[q] = 1;
        } else {
            y_test->data[q] = 0;
        }

        free(distancia);
        destroi_vetor(&indices);
        destroi_vetor(&rotulo);
    }

    return y_test;
}

int main() {
    int n_amostras_treino;
    int n_amostras_teste;
    int k; 

    scanf("%d %d %d", &n_amostras_treino, &n_amostras_teste, &k);

    Matriz *X_train = cria_matriz(n_amostras_treino, 2);

    Vetor *y_train = cria_vetor(n_amostras_treino);

    for (int p = 0; p < n_amostras_treino; p++) {
        scanf("%f %f %d", &X_train->data[p][0], &X_train->data[p][1], &y_train->data[p]);
    }

    Matriz *X_test = cria_matriz(n_amostras_teste, 2);

    for (int q = 0; q < n_amostras_teste; q++) {
        scanf("%f %f", &X_test->data[q][0], &X_test->data[q][1]);
    }

    Vetor *y_test = cria_vetor(n_amostras_teste);
    y_test = knn(X_train, y_train, X_test, k);

    for (int q = 0; q < n_amostras_teste; q++) {
        char resultado[10];
        if (y_test->data[q] == 1) {
            strcpy(resultado, "Aprovado");
        } else {
            strcpy(resultado, "Reprovado");
        }

        printf("Aluno %d: (%.2f, %.2f) = %s\n", q, X_test->data[q][0], X_test->data[q][1], resultado);
    }

    destroi_matriz(&X_train);
    destroi_vetor(&y_train);
    destroi_matriz(&X_test);
    destroi_vetor(&y_test);

    return 0;
}
