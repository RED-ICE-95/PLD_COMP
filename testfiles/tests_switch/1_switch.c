//test avec le default
int main() {
    int x;
    x = 2;

    switch (x) {
        case 1:
            x = 10;
            break;
        case 2:
            x = 20;
            break;
        case 3:
            x = 30;
            break;
        default:
            x = 0;
    }

    return x;
}