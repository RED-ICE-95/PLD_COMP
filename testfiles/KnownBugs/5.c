// GCC : accepte le shadowing d'un tableau par une variable simple dans un
//       bloc interne — retourne 10 depuis le bloc interne.
// IFCC : erreur sémantique —> notre SymbolTableVisitor interdit d'accéder à
//        a[0] comme tableau si 'a' a été redéclaré comme variable simple dans
//        un scope interne. La vérification isArrayVar ne distingue pas les
//        scopes pour ce cas précis.
int main() {
    int a[2];
    a[0] = 5;
    {
        int a = 10;
        if (a > 5) {
            return a;
        }
    }
    return a[0];
}