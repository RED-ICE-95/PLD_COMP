// Test: variables + priorité arith (sans parenthèses)
// b*2=8, b/2=2, a+8-2=9
int main() {
    int a; int b; int c;
    a = 3; b = 4;
    c = a + b * 2 - b / 2;
    return c;
}
