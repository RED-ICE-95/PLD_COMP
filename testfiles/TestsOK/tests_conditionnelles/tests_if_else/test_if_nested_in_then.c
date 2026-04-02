// if imbriqué dans le then
int main() {
    int a;
    int b;
    a = 5;
    b = 3;
    if (a > 0) {
        if (b > 0) {
            return a + b;
        } else {
            return a;
        }
    }
    return 0;
}