// GCC : retourne 300 —> i++ dans t[i++] évalue t[0] puis incrémente i à 1.
// IFCC : erreur de parsing —> même raison que le test 13, ++ n'est pas
//        supporté comme expression, donc t[i++] n'est pas reconnu par
//        notre grammaire. incdec n'existe que comme statement seul.
int main() {
    int t[3];
    t[0] = 100; t[1] = 200; t[2] = 300;
    int i = 0;
    int val = t[i++];
    if (i == 1) {
        return val + t[i];
    }
    return 0;
}