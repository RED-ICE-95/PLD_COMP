// a + b * c / d - e % f & g ^ h | i
// Full precedence chain: *, /, % first, then +/-, then &, then ^, then |
// Step 1 (*, /, %): b*c/d = 4*3/2 = 6 ; e%f = 7%3 = 1
// Step 2 (+/-): a + 6 - 1 = 5+6-1 = 10
// Step 3 (&): 10 & 15 = 1010 & 1111 = 1010 = 10
// Step 4 (^): 10 ^ 6  = 1010 ^ 0110 = 1100 = 12
// Step 5 (|): 12 | 4  = 1100 | 0100 = 1100 = 12
int main() {
    int a; int b; int c; int d; int e; int f; int g; int h; int i;
    a = 5; b = 4; c = 3; d = 2; e = 7; f = 3; g = 15; h = 6; i = 4;
    return a + b * c / d - e % f & g ^ h | i;
}
