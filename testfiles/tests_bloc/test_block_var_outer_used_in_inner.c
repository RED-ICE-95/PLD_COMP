// variable déclarée dans bloc externe et utilisée dans bloc interne
int main() {
    int a;
    a = 2;
    {
        int b;
        b = a;
    }
    return a;
}
