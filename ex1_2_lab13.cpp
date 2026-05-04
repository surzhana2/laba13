#include <cstdio>
#include <cmath>

const double EPS = 1e-9;


int gaussJordan(double **A, int M, int N) {
    int i = 0, j = 0;
    int rank = 0;
    
    while(i < M && j < N) {
        // 1. Поиск ведущего элемента (максимальный по модулю в столбце j)
        int pivot_row = i;
        for(int k = i + 1; k < M; k++) {
            if(fabs(A[k][j]) > fabs(A[pivot_row][j]))
                pivot_row = k;
        }
        
        // Если ведущий элемент близок к нулю, переходим к следующему столбцу
        if(fabs(A[pivot_row][j]) < EPS) {
            j++;
            continue;
        }
        
        // 2. Перестановка строк (если нужно)
        if(pivot_row != i) {
            for(int k = j; k <= N; k++) {
                double tmp = A[i][k];
                A[i][k] = A[pivot_row][k];
                A[pivot_row][k] = tmp;
            }
        }
        
        // 3. Нормировка текущей строки (делим на ведущий элемент)
        double pivot = A[i][j];
        for(int k = j; k <= N; k++) {
            A[i][k] /= pivot;
        }
        
        // 4. Обнуление всех остальных строк в столбце j
        for(int k = 0; k < M; k++) {
            if(k != i && fabs(A[k][j]) > EPS) {
                double factor = A[k][j];
                for(int l = j; l <= N; l++) {
                    A[k][l] -= factor * A[i][l];
                }
            }
        }
        
        i++;
        j++;
        rank++;
    }
    
    return rank;
}

int main() {
    
    FILE *fin = fopen("input.txt", "r");
    if (!fin) {
        printf("Ошибка: не удалось открыть input.txt\n");
        return 1;
    }
    
    int M, N;
    fscanf(fin, "%d%d", &M, &N);
    
    // Выделяем память под матрицу (M строк, N+1 столбцов, включая свободные члены)
    double **A = new double*[M];
    for(int i = 0; i < M; i++) {
        A[i] = new double[N + 1];
        for(int j = 0; j <= N; j++) {
            fscanf(fin, "%lf", &A[i][j]);
        }
    }
    fclose(fin);
    
    
    int rank = gaussJordan(A, M, N);
    
    // Проверка на несовместность
    bool inconsistent = false;
    for(int i = 0; i < M; i++) {
        bool allZero = true;
        for(int j = 0; j < N; j++) {
            if(fabs(A[i][j]) > EPS) {
                allZero = false;
                break;
            }
        }
        
        if(allZero && fabs(A[i][N]) > EPS) {
            inconsistent = true;
            break;
        }
    }
    
    // Выводим результат в файл
    FILE *fout = fopen("output.txt", "w");
    if (!fout) {
        printf("Ошибка: не удалось создать output.txt\n");
        return 1;
    }
    
    if(inconsistent) {
        fprintf(fout, "Система несовместна (нет решений)\n");
    }
    else if(rank == N) {
        // Единственное решение
        fprintf(fout, "Единственное решение:\n");
        for(int i = 0; i < N; i++) {
            fprintf(fout, "x%d = %.3f", i + 1, A[i][N]);
            if(i < N - 1) fprintf(fout, "; ");
        }
        fprintf(fout, "\n");
    }
    else {
        // Нет единственного решения (бесконечно много решений)
        fprintf(fout, "Система не имеет единственного решения\n");
    }
    
    fclose(fout);
    
    // Очищаем память
    for(int i = 0; i < M; i++) {
        delete[] A[i];
    }
    delete[] A;
    
    printf("Результат записан в файл output.txt\n");
    
    return 0;
}