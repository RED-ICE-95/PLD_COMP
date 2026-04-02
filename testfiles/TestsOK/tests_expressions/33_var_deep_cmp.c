// Variables + comparaisons profondes : 7 niveaux
// x=10, y=5 => ((x > y) + (x != y)) * ((x - y) > 3)
// = (1 + 1) * (5 > 3) = 2 * 1 = 2
int main() {
    int x;
    int y;
    x = 10;
    y = 5;
    return ((x > y) + (x != y)) * ((x - y) > 3);
}
