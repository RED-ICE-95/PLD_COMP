// Ce code est valide en C : on peut avoir des case après default.
// Mais notre grammaire impose que default soit en dernier (case* puis default?).
// Donc le parser rejette "case 2" après default donc erreur de syntaxe.
int main() {
    int x = 3;
    int y = 0;

    switch (x) {
        case 1:
            y = 10;
        default:
            y = 20;
        case 2:
            y = 30;
    }

    return y;
}