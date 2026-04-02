// LIMITE : affectation multiple chaînée non supportée
// En C standard, a = b = c = 5; est valide (l'affectation est une expression).
// Notre grammaire traite l'affectation comme un statement, pas une expression.
int main() {
    int a;
    int b;
    int c;
    a = b = c = 5;
    return a; // attendu : 5
}
