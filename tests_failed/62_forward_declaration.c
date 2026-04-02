// LIMITE : prototypes de fonctions (forward declarations) non supportés
// Le compilateur ne reconnaît que les définitions complètes de fonctions.
// Un prototype suivi d'une définition provoque une erreur de parsing.
int add(int a, int b);  // prototype

int main() {
    return add(3, 4); // attendu : 7
}

int add(int a, int b) {
    return a + b;
}
