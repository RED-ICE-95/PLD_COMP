// LIMITE : union non supportée
union Data {
    int i;
    char c;
};

int main() {
    union Data d;
    d.i = 65;
    return d.c; // attendu : 65
}
