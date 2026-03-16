// Test: & > ^ > | (priorité bitwise entre eux, sans parenthèses)
// 3&5=1, 2^1=3, 1|3=3
int main() {
    return 1 | 2 ^ 3 & 5;
}
