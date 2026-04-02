// LIMITE : initialisation d'un tableau avec des expressions négatives
// Les constantes négatives passent par l'unaire '-' puis la constante.
// Vérifie que la propagation de constantes gère bien les négatifs dans les listes.
int main() {
    int arr[3] = {-1, -2, -3};
    int sum = arr[0] + arr[1] + arr[2]; // -6
    // return utilise un code de retour modulo 256, donc -6 → 250
    if (sum == -6) {
        return 1;
    }
    return 0; // attendu : 1
}
