// LIMITE : pointeurs non supportés
int main() {
    int x = 42;
    int *p = &x;
    return *p; // attendu : 42
}
