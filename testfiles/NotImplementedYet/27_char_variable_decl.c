// LIMITE : déclaration de variable de type char
// La grammaire ne supporte que 'int' pour les déclarations.
// Les constantes char ('A') fonctionnent, mais pas 'char x = ...'.
int main() {
    char c = 'A';
    return c; // attendu : 65
}
