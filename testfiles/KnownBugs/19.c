// GCC : retourne 5 —> en C, l'affectation est une expression qui renvoie la valeur affectée.
// IFCC : erreur de parsing —> notre règle 'assign' est un 'stmt' et non une 'expr'.
//        L'opérateur '=' ne peut pas être chaîné car le membre de droite d'une
//        affectation doit obligatoirement être une expression mathématique ou constante.
int main() {
    int a;
    int b;
    int c;
    a = b = c = 5;
    return a;
}