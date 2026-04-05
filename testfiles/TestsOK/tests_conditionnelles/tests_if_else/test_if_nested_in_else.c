// if imbriqué dans le else
int main() {
    int a;
    a = 3;
    if (a == 1) {
        return 1;
    } else {
        if (a == 2) {
            return 2;
        } else {
            return 3;
        }
    }
}