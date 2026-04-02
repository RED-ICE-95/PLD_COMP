// LIMITE : boucle for non supportée (seul while est implémenté)
int main() {
    int sum = 0;
    for (int i = 0; i < 5; i++) {
        sum += i;
    }
    return sum; // attendu : 10
}
