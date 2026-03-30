//test avec plusieurs instructions dans un case
int main() {
    int x;
    x = 1;

    switch (x) {
        case 1: 
            x = x + 1;
            x = x + 2;
            x = x - 3
            break;
        case 2:
            x = 100;
            break;
    }

    return x;
}