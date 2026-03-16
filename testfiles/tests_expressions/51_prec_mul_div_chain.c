// a + b * c / d * e
// = a + (((b*c)/d)*e) left-to-right
// = 1 + ((3*4/2)*2) = 1 + (6*2) = 1 + 12 = 13
int main() {
    int a; int b; int c; int d; int e;
    a = 1; b = 3; c = 4; d = 2; e = 2;
    return a + b * c / d * e;
}
