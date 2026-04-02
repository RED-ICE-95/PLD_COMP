// LIMITE : variable static non supportée
int compteur() {
    static int n = 0;
    n++;
    return n;
}

int main() {
    compteur();
    compteur();
    return compteur(); // attendu : 3
}
