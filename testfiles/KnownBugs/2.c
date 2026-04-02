// GCC : accepte <= et retourne 120.
// IFCC : erreur de parsing —> notre grammaire ne supporte pas <= dans exprCmp,
//        seulement < et >. Le token <= est vu comme < suivi de = ce qui
//        déclenche une erreur syntaxique.
int factorielle(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorielle(n - 1);
}
int main() {
    return factorielle(5);
}