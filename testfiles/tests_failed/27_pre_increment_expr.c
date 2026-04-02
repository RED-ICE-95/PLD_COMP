// LIMITE : pré-incrémentation dans une expression non supportée
int main() {
    int a = 5;
    int b = ++a;
    return b; // attendu : 6
}
