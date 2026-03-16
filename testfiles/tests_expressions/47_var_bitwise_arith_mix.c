// Variables : bitwise + arith mélangés
// a=5, b=3, c=12
// (a + b) & c | (a * b - c)
// = 8 & 12 | (15 - 12)
// = 8 | 3 = 11
int main() {
    int a;
    int b;
    int c;
    a = 5;
    b = 3;
    c = 12;
    return (a + b) & c | (a * b - c);
}
