// a + b * c - d / e + f % g
// = a + (b*c) - (d/e) + (f%g)
// = 10 + 12 - 4 + 1 = 19
int main() {
    int a; int b; int c; int d; int e; int f; int g;
    a = 10; b = 3; c = 4; d = 8; e = 2; f = 7; g = 3;
    return a + b * c - d / e + f % g;
}
