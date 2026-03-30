//test avec un if dans le case d'un switch

int main() {
    int x;
    int y;
    x = 3;
    y = 0;

    switch (x) {
        case 3:
            if (y > 2) y = 10;
            else y = 5;
            break;
        case 5:
            y = 15;
            break;
        default:
            y = 20;
    }

    return y;
}