// return de la variable externe malgré shadowing interne
int main() {
    int a;
    a = 2;
    {
        int a;
        a = 99;
        return a;
    }
    return a;
}