// Test: < avant == (priorité comparaison vs égalité)
// 2+3=5, 4*2=8, 5<8=1, 1==1=1
int main() {
    return 2 + 3 < 4 * 2 == 1;
}
