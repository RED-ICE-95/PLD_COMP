// x^x = 0 toujours (XOR d'un nombre avec lui-meme)
// Non optimise dans l'IR mais resultat correct
// Attendu : 0
int main() {
    int x = 42;
    int a = x ^ x;
    return a;
}
