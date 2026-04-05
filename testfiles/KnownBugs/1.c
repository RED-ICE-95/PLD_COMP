// GCC : accepte "return a = b;" en C l'affectation est une expression
//       qui retourne la valeur affectée donc retourne 2.
// IFCC : erreur de parsing —> notre grammaire ne supporte pas l'affectation
//        comme expression dans un return. La règle assign est un stmt séparé
//        pas une expr, donc "a = b" dans return_stmt n'est pas reconnu.
int main() {
    int a;
    int b = 2;
    return a = b;
}