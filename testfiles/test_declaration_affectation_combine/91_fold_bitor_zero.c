// Element neutre bitwise OR : x|0 = x, 0|x = x
// Attendu : 6 + 6 = 12
int main() {
    int x = 6;
    int a = x | 0;
    int b = 0 | x;
    return a + b;
}
