// LIMITE : boucle do-while non supportée
int main() {
    int i = 0;
    int sum = 0;
    do {
        sum += i;
        i++;
    } while (i < 5);
    return sum; // attendu : 10
}
