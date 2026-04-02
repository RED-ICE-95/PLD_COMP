int test_mix(int a[2], int x, int b[2]) {
    int somme = a[0] + x + b[1];
    return somme;
}

int main() {
    int t1[2];
    int t2[2];
    t1[0] = 100;
    t2[1] = 50;
    
    // Appel avec : Adresse(64b), Entier(32b), Adresse(64b)
    int resultat = test_mix(t1, 10, t2);
    
    return resultat; // Doit retourner 160 (100 + 10 + 50)
}