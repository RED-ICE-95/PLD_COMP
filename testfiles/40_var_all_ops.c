// Variables avec toutes les ops : 10 niveaux
// a=6, b=3, c=2
// ( (a * b + c) % (b + 1) ) ^ ( (a > b) + (b != c) ) & (a - b)
// = (20 % 4) ^ ((1 + 1) & 3)
// = 0 ^ (2 & 3) = 0 ^ 2 = 2
int main() {
    int a;
    int b;
    int c;
    a = 6;
    b = 3;
    c = 2;
    return ((a * b + c) % (b + 1)) ^ (((a > b) + (b != c)) & (a - b));
}
