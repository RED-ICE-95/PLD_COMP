// variable intérieure calculée à partir de la variable extérieure
int main() {
    int a;
    a = 2;
    {
        int b;
        b = 3;
        {
            int c;
            c = a + b;
        }
    }
    return a;
}