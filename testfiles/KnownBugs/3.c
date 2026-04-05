// GCC : comportement indéfini —> peut retourner 10 ou 99 selon l'optimisation.
// IFCC : retourne 99 —> notre calcul d'offset place tab[0] et tab[1] de façon
//        contiguë avec cible sur la pile. tab[2] écrit à l'adresse de cible
//        car il n'y a pas de vérification des bornes. C'est un comportement
//        indéfini en C, mais révèle notre organisation mémoire sur la pile.
int main() {
    int cible = 10;
    int tab[2];
    tab[0] = 1;
    tab[1] = 2;
    tab[2] = 99;
    return cible;
}