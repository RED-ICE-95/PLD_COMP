// LIMITE : goto non supporté
int main() {
    int x = 0;
    goto fin;
    x = 1;
fin:
    return x; // attendu : 0
}
