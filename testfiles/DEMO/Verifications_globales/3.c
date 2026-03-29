int main() {
    int a;
    a = 1;
    {
        int b;
        int b; // erreur : b déjà déclaré dans CE bloc
        b = 2;
    }
    return a;
}