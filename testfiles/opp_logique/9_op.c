int main() {
    int a;
    a = 0;

    if ((a || (10 / 2)) && (a && (10 / a))) {
        return 1;
    }

    return 0;
}
// doit retourner 0