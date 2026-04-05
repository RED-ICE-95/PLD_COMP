//test avec boucle while dans un case d'un switch
int main() {
    int x;
    int y;
    x = 1;
    y = 0;

    switch (x) {
        case 1:
            while (y < 5) {
                y = y + 1;
            }
            break;
        case 2:
            y = 10;
            break;
    }

    return y;
}