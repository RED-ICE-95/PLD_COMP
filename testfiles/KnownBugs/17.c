// GCC : retourne 0 —> conforme à la norme C99 qui ajoute un return 0; implicite au main.
// IFCC : retourne une valeur indéterminée —> notre compilateur ne génère pas de
//        code de retour par défaut. Si l'utilisateur oublie 'return', le programme
//        quitte la fonction avec la valeur résiduelle du registre de retour (ex: %eax).
int main() {
    int x = 42;
}