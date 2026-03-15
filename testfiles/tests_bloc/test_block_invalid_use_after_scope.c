// b utilisé en dehors de son scope → erreur
int main() {
    int a;
    a = 1;
    {
        int b;
        b = 2;
    }
    return b;
}
