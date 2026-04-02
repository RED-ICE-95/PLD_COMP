// LIMITE : nombre négatif comme constante de case dans un switch
// Les case du switch n'acceptent que des CONST [0-9]+, pas de négatifs.
int main() {
    int x = -1;
    switch (x) {
        case -1:
            return 1;
        case 0:
            return 2;
    }
    return 0; // attendu : 1
}
