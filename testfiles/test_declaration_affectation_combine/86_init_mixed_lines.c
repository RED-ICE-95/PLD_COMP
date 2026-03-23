// Mix de lignes avec et sans init, calcul final combine tout
// int a = 2, b; int c = a * 3; b = c + 1; return b;
// a=2, c=6, b=7
// Attendu : 7
int main() {
    int a = 2, b;
    int c = a * 3;
    b = c + 1;
    return b;
}
