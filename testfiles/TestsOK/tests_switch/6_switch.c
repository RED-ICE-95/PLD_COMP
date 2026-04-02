//test avec switch case imbriqués

int main() {
    int x;
    int y;
    x = 1;
    y = 0;

    switch (x) {
        case 1:
            switch (x + 1) {
                case 2:
                    y = 10;
                    break;
                default:
                    y = 20;
            }
            break;
        default:
            y = 30;
    }

    return y;
}