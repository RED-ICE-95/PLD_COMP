int main() {
    int a;
    a = 1;
    {
        int b;
        b = 2;
    }
    return b; // erreur : b n'existe plus, son scope est fermé
}