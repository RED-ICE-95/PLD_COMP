// if imbriqué dans le then
int main() {
    int a;
    a = 5;
    if (a > 0) {
        if (a > 3) {
            a = 100;
        } else {
            a = 50;
        }
    }
    return a;
}