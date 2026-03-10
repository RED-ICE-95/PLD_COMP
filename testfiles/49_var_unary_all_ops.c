// Variables : unary + toutes ops mélangées sur 8 niveaux
// a=4, b=6, c=2
// !(a > b) * (a ^ b) + -(c - a) * (b & c)
// = 1 * (4^6) + -(-2) * (6&2)
// = 1 * 2 + 2 * 2 = 6
int main() {
    int a;
    int b;
    int c;
    a = 4;
    b = 6;
    c = 2;
    return !(a > b) * (a ^ b) + -(c - a) * (b & c);
}
