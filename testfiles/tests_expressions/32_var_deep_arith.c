// Variables + arithmetique profonde : 7 niveaux
// a=3, b=4, c=5 => ((a*b) + (b*c) - (a+c)) / a
// = (12 + 20 - 8) / 3 = 24 / 3 = 8
int main() {
    int a;
    int b;
    int c;
    a = 3;
    b = 4;
    c = 5;
    return ((a * b) + (b * c) - (a + c)) / a;
}
