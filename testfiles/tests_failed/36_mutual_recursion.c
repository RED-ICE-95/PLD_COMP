// LIMITE : récursion mutuelle
// La fonction 'est_pair' appelle 'est_impair' qui appelle 'est_pair'.
// Le compilateur exige que la fonction appelée soit déclarée AVANT.
// En C standard, une forward declaration suffirait, mais notre compilateur
// ne supporte pas les prototypes de fonctions (forward declarations).
int est_impair(int n);

int est_pair(int n) {
    if (n == 0) return 1;
    return est_impair(n - 1);
}

int est_impair(int n) {
    if (n == 0) return 0;
    return est_pair(n - 1);
}

int main() {
    return est_pair(4); // attendu : 1
}
