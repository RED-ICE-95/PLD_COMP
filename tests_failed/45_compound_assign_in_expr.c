// LIMITE : affectation composée dans une expression
// a += 3 est une instruction (statement), pas une expression.
// On ne peut pas écrire : b = (a += 3);
int main() {
    int a = 2;
    int b = (a += 3);
    return b; // attendu : 5
}
