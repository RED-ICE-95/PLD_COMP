// GCC : retourne 1 —> les constantes négatives sont des expressions valides pour un case.
// IFCC : erreur de parsing —> la règle 'case_stmt' attend le jeton 'CONST', 
//        défini par le Lexer comme [0-9]+ (entiers positifs uniquement). 
//        Le signe '-' est vu comme un opérateur soustraction, invalide ici.
int main() {
    int x = -1;
    switch (x) {
        case -1:
            return 1;
        case 0:
            return 2;
    }
    return 0;
}