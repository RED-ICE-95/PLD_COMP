int main() {
    int x;
    x = 0;

    while (1) {
        x = x + 1;
        break;
        x = 100; // jamais exécuté
    }

    return x;
}