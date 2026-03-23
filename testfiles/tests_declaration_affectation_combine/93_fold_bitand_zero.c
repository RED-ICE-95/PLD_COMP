// Element absorbant bitwise AND : x&0 = 0, 0&x = 0
// Attendu : 0 + 0 = 0
int main() {
    int x = 15;
    int a = x & 0;
    int b = 0 & x;
    return a + b;
}
