// return dans bloc intérieur, le return externe ne doit pas s'exécuter
int main() {
    int a;
    a = 2;
    {
        int b;
        b = 3;
        return b;
    }
    return a;
}
