// Element neutre multiplication : x*1 et 1*x donnent x
// x*0 et 0*x donnent 0
// Attendu : 5 + 5 + 0 + 0 = 10
int main() {
    int x = 5;
    int a = x * 1;
    int b = 1 * x;
    int c = x * 0;
    int d = 0 * x;
    return a + b + c + d;
}
