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