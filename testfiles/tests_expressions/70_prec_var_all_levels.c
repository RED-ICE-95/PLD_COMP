// Test: variables + tous les niveaux de priorité sans parenthèses
// a*b=12, 12+c-1=13, 13&b=1 (1101&0011), 1^a=5 (001^100), c+1=3, 5|3=7
int main() {
    int a; int b; int c; int d;
    a = 4; b = 3; c = 2;
    d = a * b + c - 1 & b ^ a | c + 1;
    return d;
}
