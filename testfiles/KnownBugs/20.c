// GCC : retourne 5 —> affecte 5 à x, puis évalue l'expression (5 est vrai).
// IFCC : erreur de parsing —> notre grammaire définit 'assign' comme un 'stmt'
//        indépendant. Les parenthèses d'un 'if' attendent une 'expr', laquelle
//        ne contient pas de règle pour l'opérateur d'affectation '='.
int main() {
    int x = 0;
    if (x = 5) {
        return x;
    }
    return 0;
}