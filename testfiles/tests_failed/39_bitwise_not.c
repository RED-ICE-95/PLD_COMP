// LIMITE : opérateur bitwise NOT ~ non supporté
// La grammaire ne reconnaît que ! (NOT logique), pas ~ (NOT bit-à-bit).
int main() {
    int x = 0;
    int y = ~x;
    return y; // attendu : -1 (tous les bits à 1)
}
