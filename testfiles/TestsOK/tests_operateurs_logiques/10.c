int main() {
    int a;
    int b;
    int res;
    a = 4;
    b = 2;
    res = 0;

    while (a && b) {
        res = res + 1;
        a = a - 1;
        b = b - 1;
    }

    return res;
}
// attendu : 2