// Test: variables + priorité bitwise (sans parenthèses)
// c&a = 3&5=1, b^1 = 6^1=7, a|7 = 5|7=7
int main() {
    int a; int b; int c; int d;
    a = 5; b = 6; c = 3;
    d = a | b ^ c & a;
    return d;
}
