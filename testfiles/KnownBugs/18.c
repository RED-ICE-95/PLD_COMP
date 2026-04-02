// GCC : retourne 1 —> utilise le prototype pour résoudre l'appel avant la définition.
// IFCC : erreur de parsing —> notre grammaire impose que 'fonctDecl' possède un corps {}.
//        Les prototypes (forward declarations) ne sont pas supportés. De plus,
//        notre compilateur nécessite que la fonction soit connue avant d'être appelée.
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
    return est_pair(4);
}