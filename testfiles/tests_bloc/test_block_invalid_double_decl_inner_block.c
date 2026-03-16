// double déclaration dans le même bloc intérieur → erreur
int main() {
    int a;
    a = 1;
    {
        int b;
        int b;
        b = 2;
    }
    return a;
}