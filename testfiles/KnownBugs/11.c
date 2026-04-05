// GCC x86 : retourne 0 —> la mémoire non initialisée vaut 0 en pratique.
// MSP430 + mspsim : retourne 255 — la RAM est initialisée à 0xffff,
//                   arr[2] lit 0xffff, masqué à 8 bits = 255.
// IFCC : retourne 88 —> comportement indéfini, notre calcul d'offset fait
//        que arr[2] lit une variable temporaire ou une autre variable locale
//        qui contient 88 à ce moment. Résultat imprévisible et non comparable.
int main() {
    int arr[2] = {5, 10};
    int x = arr[2];
    return x;
}