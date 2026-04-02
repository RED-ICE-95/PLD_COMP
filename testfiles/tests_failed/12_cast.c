// LIMITE : cast explicite non supporté
int main() {
    char c = 'A';
    int x = (int)c;
    return x; // attendu : 65
}
