// GCC : accepte b = c = 3 et a = 2 + (b = c) —> l'affectation est une
//       expression en C qui retourne la valeur affectée permettant le chaînage.
// IFCC : erreur de parsing —> notre grammaire ne supporte pas l'affectation
//        comme expression. assign est un stmt séparé, pas une expr,
//        donc "b = c = 3" et "(b = c)" dans une expression ne sont pas reconnus.
int main() {
    int a, b, c;
    b = c = 3;
    a = 2 + (b = c);
    return 0;
}