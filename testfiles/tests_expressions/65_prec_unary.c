// Test: unaire avant toutes les ops binaires (sans parenthèses)
// -3 + 5*2 => unaire appliqué à 3 seulement, pas à tout: -3 + 10 = 7
int main() {
    return -3 + 5 * 2;
}
