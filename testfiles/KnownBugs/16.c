// GCC : retourne 11 —> t[i++] lit t[0]=10 puis i devient 1, retourne 10+1=11.
// IFCC : erreur de parsing —> même cause que les tests 13, 14 et 15.
//        ++ dans un indice de tableau n'est pas une expression valide dans
//        notre grammaire. La limitation est globale : ++ et -- ne sont
//        supportés que comme statements autonomes, jamais comme expressions.
int main() {
    int t[2];
    t[0] = 10; t[1] = 20;
    int i = 0;
    int val = t[i++];
    return val + i;
}