// -a * b + c
// unary minus binds tighter than *: (-a) * b + c
// = (-2) * 3 + 10 = -6 + 10 = 4
int main() {
    int a; int b; int c;
    a = 2; b = 3; c = 10;
    return -a * b + c;
}
