// GCC : retourne 1 —> évalue l'expression constante '2+3' à la compilation (5).
// IFCC : erreur de parsing —> par simplicité, notre règle 'case_stmt' n'accepte
//        qu'une constante littérale 'CONST'. Elle n'autorise pas la règle 'expr'
//        (qui permettrait '2+3'), empêchant tout calcul au sein des labels du switch.
int main() {
    int x = 5;
    switch (x) {
        case 2+3:
            return 1;
        default:
            return 0;
    }
}