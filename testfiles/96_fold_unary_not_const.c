// Fold unary not sur constante : !0 = 1, !5 = 0, !!1 = 1
// Attendu : 1 + 0 + 1 = 2
int main() {
    int a = !0;
    int b = !5;
    int c = !!1;
    return a + b + c;
}
