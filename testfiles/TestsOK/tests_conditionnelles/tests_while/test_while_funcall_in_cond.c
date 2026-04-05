// appel de fonction dans la condition d'un while
// Vérifie que l'évaluation de la condition est correcte quand
// la condition elle-même fait un appel de fonction.
int decrement(int x) {
    return x - 1;
}

int main() {
    int i = 5;
    int sum = 0;
    while (decrement(i) > 0) {
        sum += i;
        i = i - 1;
    }
    return sum; // attendu : 5+4+3+2 = 14
}
