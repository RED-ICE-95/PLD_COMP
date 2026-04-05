// Ce fichier DOIT générer des erreurs

int add(int a, int b) {
    return a + b;
}

int main() {
    int x;
    
    // ERREUR : add attend 2 arguments, 1 fourni
    x = add(5);
    
    return x;
}
