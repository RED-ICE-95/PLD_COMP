// deux if séquentiels indépendants
int main() {
    int a;
    a = 0;
    if (1) {
        a = a + 1;
    }
    if (1) {
        a = a + 1;
    }
    return a;
}
