// Element neutre addition : x+0 et 0+x doivent donner x
// Attendu : 7 + 7 = 14... non : on retourne x+0 donc 7
int main() {
    int x = 7;
    int a = x + 0;
    int b = 0 + x;
    return a + b;
}
// 7 + 7 = 14
