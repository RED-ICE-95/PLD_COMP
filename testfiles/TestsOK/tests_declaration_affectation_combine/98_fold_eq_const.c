// Fold egalite sur constantes
// 3 == 3 = 1, 3 == 4 = 0, 3 != 4 = 1, 3 != 3 = 0
// Attendu : 1 + 0 + 1 + 0 = 2
int main() {
    int a = 3 == 3;
    int b = 3 == 4;
    int c = 3 != 4;
    int d = 3 != 3;
    return a + b + c + d;
}
