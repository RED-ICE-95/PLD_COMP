// LIMITE : comparaison >= non supportée dans la grammaire
// (seuls < et > sont implémentés)
int main() {
    int a = 5;
    int b = 3;
    if (a >= b) {
        return 1;
    }
    return 0; // attendu : 1
}
