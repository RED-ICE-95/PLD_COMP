// bloc interne sans return, le return externe s'exécute
int main() {
    int a;
    a = 5;
    {
        int b;
        b = 3;
    }
    return a;
}