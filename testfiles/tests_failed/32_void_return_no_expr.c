// LIMITE : return sans expression dans une fonction void
// La grammaire exige 'return expr;', donc
// 'return;' seul provoque une erreur de parsing.
void foo() {
    int x = 1;
    return;
}

int main() {
    foo();
    return 0;
}
