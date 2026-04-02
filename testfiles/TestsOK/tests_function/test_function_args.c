// Test de vérification du nombre d'arguments

int add(int a, int b) {
    return a + b;
}

int main() {
    int x;
    
    // Correct : getchar prend 0 arguments
    x = getchar();
    
    // Correct : putchar prend 1 argument  
    putchar(x);
    
    // Correct : add prend 2 arguments
    x = add(3, 5);
    
    // ERREUR attendue : putchar() sans argument
    // putchar();
    
    // ERREUR attendue : putchar avec 2 arguments
    // putchar(65, 66);
    
    // ERREUR attendue : add avec 1 seul argument
    // x = add(3);
    
    return 0;
}
