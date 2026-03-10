// a + b * c & d + e
// arith binds tighter than bitwise: (a + b*c) & (d + e)
// = (1 + 12) & (10 + 5) = 13 & 15 = 1101 & 1111 = 13
int main() {
    int a; int b; int c; int d; int e;
    a = 1; b = 3; c = 4; d = 10; e = 5;
    return a + b * c & d + e;
}
