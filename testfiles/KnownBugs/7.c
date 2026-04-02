// GCC : retourne 0 —> l'affectation dans une condition est une expression
//       valide en C. Comme a vaut 0, le && court-circuite et check reste 0.
// IFCC : erreur de parsing —> comme le test 1, notre grammaire ne supporte
//        pas l'affectation comme expression. "check = increment_check(check)"
//        dans une condition n'est pas reconnu.

int increment_check(int n) {
    return n + 1;
}

int main() {
    int a = 0;
    int check = 0;

    if (a && (check = increment_check(check))) {
        return 99; 
    }
    
    return check; 
}