// LIMITE : opérateur ternaire non supporté
int main() {
    int a = 5;
    int b = (a > 3) ? 1 : 0;
    return b; // attendu : 1
}
