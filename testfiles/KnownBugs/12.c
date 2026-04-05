// GCC : putchar est accepté même sans argument.
// IFCC : erreur détectée correctement —> notre SymbolTableVisitor vérifie
//        le nombre d'arguments et émet "fonction 'putchar' attend 1 argument
//        mais 0 fourni". 
int main() {
    putchar();
    return 0;
}