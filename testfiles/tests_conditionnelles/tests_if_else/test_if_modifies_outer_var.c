// le if modifie une variable déclarée avant
int main() {
    int a;
    int b;
    a = 10;
    b = 0;
    if (a > 5) {
        b = 1;
    }
    return b;
}