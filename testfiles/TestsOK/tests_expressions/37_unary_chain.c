// Chaine unaire + arith : 8 niveaux
// -( -(2 + 3) ) + !( !(4 > 3) ) * 10
// = 5 + !(0) * 10 = 5 + 1 * 10 = 15
int main() {
    return -(-(2 + 3)) + !(!(4 > 3)) * 10;
}
