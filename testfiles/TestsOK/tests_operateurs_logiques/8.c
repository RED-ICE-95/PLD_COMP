int main() {
    int a;
    a = 3;

    while ((a && (a - 1)) || (a == 1)) {
        a = a - 1;
    }

    return a;
}
// doit retourner 0