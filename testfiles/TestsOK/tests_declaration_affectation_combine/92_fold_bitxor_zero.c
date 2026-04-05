// Element neutre bitwise XOR : x^0 = x, 0^x = x
// Attendu : 11 + 11 = 22
int main() {
    int x = 11;
    int a = x ^ 0;
    int b = 0 ^ x;
    return a + b;
}
