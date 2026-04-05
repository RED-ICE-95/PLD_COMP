// GCC : accepte "return;" dans une fonction void — retour sans valeur valide.
// IFCC : erreur de parsing —> notre règle return_stmt impose toujours une expr
//        après return. Un return vide n'est pas supporté dans notre grammaire.
void test_void(int n) {
    if (n > 10) {
        return;
    }
    putchar(65);
}
int main() {
    test_void(15);
    return 0;
}