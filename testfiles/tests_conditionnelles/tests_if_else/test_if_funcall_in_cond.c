// LIMITE : appel de fonction dans la condition d'un if
// avec effets de bord (la fonction modifie un état global ou un résultat)
int check(int x) {
    if (x > 10) {
        return 1;
    }
    return 0;
}

int main() {
    int r = 0;
    if (check(15) == 1) {
        r = 42;
    }
    return r; // attendu : 42
}
