// LIMITE : enum non supporté
enum Color { RED, GREEN, BLUE };

int main() {
    enum Color c = GREEN;
    return c; // attendu : 1
}
