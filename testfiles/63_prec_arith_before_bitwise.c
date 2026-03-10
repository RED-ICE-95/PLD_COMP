// Test: arith avant bitwise (sans parenthèses)
// 2+3*2=8, 5*2=10, 8&10=8 (1000&1010), 8|1=9
int main() {
    return 2 + 3 * 2 & 5 * 2 | 1;
}
