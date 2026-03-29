int main() {
    int a;
    a = 5; // On marque "a" comme utilisé ICI
    {
        int a; // Ce "a" n'est JAMAIS utilisé
    }
    return 0;
}