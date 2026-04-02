// LIMITE : post-incrémentation dans une expression non supportée
// (seul x++ en tant qu'instruction autonome est supporté)
int main() {
    int a = 5;
    int b = a++;
    return b; // attendu : 5 (valeur avant incrément)
}
