// Egalite et inegalite profonds : 8 niveaux
// ((5 != 3) == (4 == 4)) + ((1 != 1) == (2 != 3))
// = (1 == 1) + (0 == 1) = 1 + 0 = 1
int main() {
    return ((5 != 3) == (4 == 4)) + ((1 != 1) == (2 != 3));
}
