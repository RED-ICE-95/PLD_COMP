// GCC : retourne 22 —> supporte ++ et -- comme expressions avec valeur de
//       retour (post-incrémentation retourne l'ancienne valeur,
//       pré-incrémentation retourne la nouvelle).
// IFCC : erreur de parsing —> notre grammaire définit incdec comme un stmt
//        autonome (ID op=('++' | '--') ';'), pas comme une expression.
//        Donc a++ ne peut pas apparaître dans int res_post = a++ ni comme
//        opérande d'une expression.
int main() {
    int a = 10;
    int b = 10;
    int res_post = a++;
    int res_pre = ++b;
    if (res_post == 10) {
        if (res_pre == 11) {
            return a + b;
        }
    }
    return 0;
}