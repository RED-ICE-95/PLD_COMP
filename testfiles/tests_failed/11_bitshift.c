// LIMITE : opérateurs de décalage bit-à-bit non supportés
int main() {
    int x = 1;
    int y = x << 3;
    int z = y >> 1;
    return z; // attendu : 4
}
