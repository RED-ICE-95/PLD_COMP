// GCC : retourne 2 correctement.
// IFCC : segmentation fault —> même cause que le test 6. Le passage d'un
//        tableau en paramètre à travers plusieurs niveaux de récursion
//        échoue dans notre génération de code. L'adresse de base du tableau
//        est mal calculée lors des appels récursifs imbriqués.
int chercher(int t[], int taille, int cible) {
    if (taille < 0) return 0;
    if (t[taille] == cible) return taille;
    return chercher(t, taille - 1, cible);
}
int main() {
    int donnees[4];
    donnees[0] = 10; donnees[1] = 50;
    donnees[2] = 100; donnees[3] = 200;
    return chercher(donnees, 3, 100);
}