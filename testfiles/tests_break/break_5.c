int main() {
    int x;
    x = 0;

    while (1) {
        switch (x) {
            case 2:
                x = 100;
                break;
        }

        if (x == 100)
            break;

        x = x + 1;
    }

    return x;
}