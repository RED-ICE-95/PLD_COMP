// condition fausse, branche then ignorée
int main() {
    int a;
    a = 0;
    if (a) {
        a = 42;
    }
    return a;
}