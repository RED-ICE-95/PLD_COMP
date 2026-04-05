// plusieurs instructions dans le bloc then
int main() {
    int a;
    int b;
    int c;
    a = 1;
    b = 2;
    c = 0;
    if (a < b) {
        c = a + b;
        a = c * 2;
        b = a - 1;
    }
    return b;
}