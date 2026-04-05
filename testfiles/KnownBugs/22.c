// GCC : retourne 7 —> le prototype informe le compilateur de la signature de 'add'.
// IFCC : erreur de parsing —> la règle 'fonctDecl' impose la présence d'un 'block' {}.
//        Une signature de fonction suivie d'un ';' n'est pas reconnue par la grammaire,
//        rendant impossible la déclaration sans définition immédiate.
int add(int a, int b);

int main() {
    return add(3, 4);
}

int add(int a, int b) {
    return a + b;
}