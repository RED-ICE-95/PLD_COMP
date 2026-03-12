// Init avec unary minus
// int x = -5; return x;
// Attendu : 251 (en unsigned char) mais ici on retourne int donc : -5 -> 251 via exit code
// En pratique le shell retourne 251 pour -5 (255+1 = 256, -5 + 256 = 251)
// Attendu : 251
int main() {
    int x = -5;
    return x;
}
