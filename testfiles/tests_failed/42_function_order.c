// LIMITE : l'ordre de définition des fonctions est important
// Si f() appelle g() et que g() est définie après f(),
// le compilateur ne connaît pas g() au moment de compiler f().
int f() {
    return g(); // g n'est pas encore déclarée → erreur
}

int g() {
    return 42;
}

int main() {
    return f(); // attendu : 42
}
