// Mix maximal : arith + bitwise + cmp + unary sur 10 niveaux
// a=8, b=3, c=5
// ((a / b) * c + !(a == b)) ^ ((a & c) | (b * c - a)) + -(b > c) * (a % b)
// = (2*5 + 1) ^ ((8&5) | (15-8)) + -(0) * 2
// = 11 ^ (0 | 7) + 0
// = 11 ^ 7 = 12
int main() {
    int a;
    int b;
    int c;
    a = 8;
    b = 3;
    c = 5;
    return (((a / b) * c + !(a == b)) ^ ((a & c) | (b * c - a))) + -(b > c) * (a % b);
}
