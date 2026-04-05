// GCC : retourne 11 —> x++ retourne 5 puis x devient 6, ++y incrémente
//       d'abord y à 6 puis retourne 6. res = 5 + 6 = 11.
// IFCC : erreur de parsing —> ++ dans une expression arithmétique n'est
//        pas supporté. Notre grammaire exige que incdec soit un statement
//        seul terminé par ';', pas un opérateur dans une expression.
int main() {
    int x = 5;
    int y = 5;
    int res = x++ + ++y;
    if (x == 6) {
        if (y == 6) {
            return res;
        }
    }
    return 0;
}