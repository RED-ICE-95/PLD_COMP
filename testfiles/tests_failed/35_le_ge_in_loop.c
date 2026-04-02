// LIMITE : while avec condition <=
// La boucle classique 'while (i <= n)' ne fonctionne pas car
// la grammaire n'inclut pas <= dans les opérateurs de comparaison.
int main() {
    int sum = 0;
    int i = 1;
    while (i <= 10) {
        sum += i;
        i++;
    }
    return sum % 256; // attendu : 55
}
