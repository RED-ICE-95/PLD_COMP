//  vérifie que le return à l'intérieur du then termine bien la fonction immédiatement et retourne 20
int main() {
    int a;
    int b;

    a = 4;
    b = 0;

    if (a > 3) {
        b = 20;
        return b;
    }

    return 0;
}