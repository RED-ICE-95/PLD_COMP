// Mix : fold partiel (une constante, une variable)
// x*1 → x, x+0 → x, 0+x → x
// Attendu : 8 + 8 + 8 = 24
int main() {
    int x = 8;
    int a = x * 1;
    int b = x + 0;
    int c = 0 + x;
    return a + b + c;
}
