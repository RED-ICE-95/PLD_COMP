#include <stdio.h>

/* Fonction avec paramètres et retour */
int max(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

/* Fonction utilisant while + opérations */
int puissance(int base, int exp) {
    int result = 1;
    while (exp > 0) {
        result = result * base;
        exp = exp - 1;
    }
    return result;
}

/* Fonction avec return au milieu */
int test_return(int x) {
    if (x < 0) {
        return -1;
    }
    if (x == 0) {
        return 0;
    }
    return 1;
}

int main() {
    /* Variables et constantes */
    int a = 5;
    int b = 3;
    int c;

    /* Affectation avec expression */
    c = a + b * 2;

    /* Comparaisons + if/else */
    if (c > 10) {
        putchar('A');
    } else {
        putchar('B');
    }

    /* Boucle while */
    int i = 0;
    while (i < 5) {
        putchar('0' + i);
        i = i + 1;
    }

    /* Division et modulo */
    int div = c / b;
    int mod = c % b;

    /* Opérations bit-à-bit */
    int and_op = a & b;
    int or_op = a | b;
    int xor_op = a ^ b;

    /* Opérations unaires */
    int neg = -a;
    int not_logique = !a;

    /* Appel de fonctions */
    int m = max(a, b);
    int p = puissance(2, 4);

    /* Affectation imbriquée */
    int x;
    int y;
    y = 10;
    x = y;

    /* Shadowing */
    int shadow = 1;
    {
        int shadow = 2;
        putchar('0' + shadow);
    }
    putchar('0' + shadow);

    /* Test return */
    int t = test_return(-5);

    /* Lecture avec getchar */
    putchar('\n');
    putchar('>');
    int ch = getchar();

    if (ch == 'q') {
        return 0;
    }

    /* Expression complexe */
    int complex = (a + b) * (c - 2) / (1 + (a > b)) % 7;

    /* Utilisation finale */
    putchar('\n');
    putchar('0' + (complex % 10));

    return 0;
}