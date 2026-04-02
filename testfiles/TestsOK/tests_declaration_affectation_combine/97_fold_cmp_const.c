// Fold comparaisons sur constantes
// 3 < 5 = 1, 5 < 3 = 0, 3 > 5 = 0, 5 > 3 = 1
// Attendu : 1 + 0 + 0 + 1 = 2
int main() {
    int a = 3 < 5;
    int b = 5 < 3;
    int c = 3 > 5;
    int d = 5 > 3;
    return a + b + c + d;
}
