#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

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
    int n;

    if (argc > 1) {
        if (isNumber(argv[1])) {
            n = atoi(argv[1]);
        } else {
            printf("Error: El parámetro ingresado para 'n' no es un número válido.\n");
            return 1;
        }
    }

    double factor = 1.0;
    double sum = 0.0;
    double pi_approx = 0.0;

    for (int k = 0; k < n; k++) {
        sum += factor/(2*k+1);
        factor = -factor;
    }
    pi_approx = 4.0*sum;

    printf("Aproximación de PI: %.15f\nn = %d\n", pi_approx, n);

    return 0;

}