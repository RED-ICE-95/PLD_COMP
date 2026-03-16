// !a + b * c
// unary ! binds tightest: (!a) + (b*c)
// a=0: !0 = 1
// = 1 + 3*4 = 1 + 12 = 13
int main() {
    int a; int b; int c;
    a = 0; b = 3; c = 4;
    return !a + b * c;
}
