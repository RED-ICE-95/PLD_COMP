int main() {
    int x;
    x = 0;

    while (1) {
        x = x + 1;

        if (x < 5)
            continue;

        break;
    }

    return x;
}