// GCC : retourne 42 —> capable de résoudre les symboles via les prototypes ou 
//        plusieurs passes de compilation.
// IFCC : erreur sémantique —> notre compilateur effectue une analyse en une seule passe.
//        Au moment de compiler f(), la fonction g() n'est pas encore présente dans 
//        la table des symboles, ce qui génère une erreur "Undeclared function".
int f() {
    return g();
}

int g() {
    return 42;
}

int main() {
    return f();
}