// LIMITE : long non supporté
int main() {
    long x = 100000;
    return (int)(x % 256); // attendu : 160
}
