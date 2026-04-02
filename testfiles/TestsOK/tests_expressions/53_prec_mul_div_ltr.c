// a * b / c * d / e
// strictly left-to-right: ((((a*b)/c)*d)/e)
// = 12*5/3*4/8 = 60/3*4/8 = 20*4/8 = 80/8 = 10
int main() {
    int a; int b; int c; int d; int e;
    a = 12; b = 5; c = 3; d = 4; e = 8;
    return a * b / c * d / e;
}
