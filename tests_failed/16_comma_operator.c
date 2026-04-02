// LIMITE : opérateur virgule non supporté
int main() {
    int a;
    a = (1, 2, 3);
    return a; // attendu : 3
}
