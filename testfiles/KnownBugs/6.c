// GCC : retourne 60 correctement.
// IFCC : segmentation fault —> le passage de tableau en paramètre (int t[])
//        est partiellement supporté mais l'accès récursif t[n] dans les
//        appels imbriqués corrompt les adresses. Le pointeur vers le tableau
//        n'est pas correctement transmis à travers les appels récursifs
//        dans notre génération de code.
int somme_recursive(int t[], int n) {
    if (n < 1) {
        return t[0];
    }
    return t[n] + somme_recursive(t, n - 1);
}
int main() {
    int chiffres[3];
    chiffres[0] = 10;
    chiffres[1] = 20;
    chiffres[2] = 30;
    return somme_recursive(chiffres, 2);
}