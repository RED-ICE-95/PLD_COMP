// Variables + bitwise profond : 8 niveaux
// a=0b1010=10, b=0b1100=12, c=0b0110=6
// ((a & b) | c) ^ (a | (b & c))
// = (8 | 6) ^ (10 | 4) = 14 ^ 14 = 0
int main() {
    int a;
    int b;
    int c;
    a = 10;
    b = 12;
    c = 6;
    return ((a & b) | c) ^ (a | (b & c));
}
