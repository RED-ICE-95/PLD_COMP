// a + b * c < d - e
// arith binds tighter than comparison: (a + b*c) < (d - e)
// = (2 + 6) < (20 - 5) = 8 < 15 = 1
int main() {
    int a; int b; int c; int d; int e;
    a = 2; b = 3; c = 2; d = 20; e = 5;
    return a + b * c < d - e;
}
