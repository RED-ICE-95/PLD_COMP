// Test: arith > bitwise > cmp, tous mélangés sans parenthèses
// 5*2=10, 3+10=13, 1&13=1, 2^1=3, 4|3=7
int main() {
    return 4 | 2 ^ 1 & 3 + 5 * 2;
}
