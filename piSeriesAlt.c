#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <omp.h>
#include <time.h>

bool isNumber(const char *str) {
    while (*str) {
        if (!isdigit(*str)) {
            return false;
        }
        str++;
    }
    return true;
}

int main(int argc, char *argv[]){
    int n, thread_count;
    
    if (argc > 1) {
        if (isNumber(argv[1])) {
            thread_count = atoi(argv[1]);
        } else {
            printf("Error: número de threads no es válido. Utilizando valor predeterminado.\n");
            thread_count = 10;
        }
        if (argc > 2 && isNumber(argv[2])) {
            n = atoi(argv[2]);
        } else {
            printf("Error: El parámetro ingresado para 'n' no es un número válido.\n");
            return 1;
        }
    }

    double sum = 0.0;
    double pi_approx = 0.0;

    clock_t start = clock();

    #pragma omp parallel for num_threads(thread_count) reduction(+:sum) schedule(guided, 128)
    for (int k = 0; k < n; k+=2) {
        sum += 1.0/(2*k+1);
    }

    #pragma omp parallel for num_threads(thread_count) reduction(+:sum) schedule(guided, 128)
    for (int k = 1; k < n; k+=2) {
        sum -= 1.0/(2*k+1);
    }
    
    pi_approx = 4.0*sum;

    clock_t end = clock();
    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Aproximación de PI: %.15f\nthreads = %d\nn = %d\ntiempo = %.6f segundos\n", pi_approx, thread_count, n, time_taken);

    return 0;
}