int main() {
    int x;
    int sum;

    x = 0;
    sum = 0;

    while (x < 6) {
        x = x + 1;

        if (x == 2)
            continue;

        if (x == 5)
            break;

        sum = sum + x;
    }

    return sum;
}