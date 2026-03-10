// Mix : arith + bitwise + cmp + unary sur 7 niveaux
// ( (3 * 4) & (5 + 7) ) | ( (2 == 2) * 3 )
// = (12 & 12) | (1 * 3) = 12 | 3 = 15
int main() {
    return ((3 * 4) & (5 + 7)) | ((2 == 2) * 3);
}
