// LIMITE : paramètre de type char non supporté (seul int est accepté)
char identity(char c) {
    return c;
}

int main() {
    return identity('A'); // attendu : 65
}
