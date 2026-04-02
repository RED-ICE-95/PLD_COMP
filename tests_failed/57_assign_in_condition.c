// LIMITE : affectation dans la condition d'un if
// En C standard, if (x = 5) est valide (affecte puis teste).
// Notre grammaire ne supporte pas l'affectation dans une expression.
int main() {
    int x = 0;
    if (x = 5) {
        return x;  // attendu : 5
    }
    return 0;
}
