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

void calculate_pi(int n, int thread_count, int block_size) {
    double sum = 0.0;
    double pi_approx = 0.0;
    double factor = 1.0;

    clock_t start = clock();

    // #pragma omp parallel for num_threads(thread_count) reduction(+:sum) private(factor) schedule(static, block_size)
    // #pragma omp parallel for num_threads(thread_count) reduction(+:sum) private(factor) schedule(dynamic, block_size)
    // #pragma omp parallel for num_threads(thread_count) reduction(+:sum) private(factor) schedule(guided, block_size)
    #pragma omp parallel for num_threads(thread_count) reduction(+:sum) private(factor) schedule(auto)
    for (int k = 0; k < n; k++) {
        factor = (k % 2 == 0) ? 1.0 : -1.0;
        sum += factor/(2*k+1);
    }

    pi_approx = 4.0 * sum;

    clock_t end = clock();
    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;

    // printf("Aproximación de PI: %.15f\nthreads = %d\nn = %d\nblock_size = %d\ntiempo = %.6f segundos\n",
    //        pi_approx, thread_count, n, block_size, time_taken);
    printf("Aproximación de PI: %.15f\nthreads = %d\nn = %d\ntiempo = %.6f segundos\n",
           pi_approx, thread_count, n, time_taken);
}

int main(int argc, char *argv[]) {
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

    int block_sizes[] = {16, 64, 128};
    int num_block_sizes = sizeof(block_sizes) / sizeof(block_sizes[0]);

    // for (int i = 0; i < num_block_sizes; i++) {
    //     calculate_pi(n, thread_count, block_sizes[i]);
    // }

    // Auto scheduling
    calculate_pi(n, thread_count, 0);

    return 0;
}
