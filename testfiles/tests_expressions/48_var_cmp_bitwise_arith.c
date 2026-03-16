// Variables : cmp + bitwise + arith
// a=7, b=3
// (a > b) * (a & b) + (a - b) * (a != b)
// = 1 * 3 + 4 * 1 = 7
int main() {
    int a;
    int b;
    a = 7;
    b = 3;
    return (a > b) * (a & b) + (a - b) * (a != b);
}
