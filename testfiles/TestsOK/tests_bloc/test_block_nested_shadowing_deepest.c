// shadowing au 3ème niveau, les niveaux supérieurs non affectés
int main() {
    int a;
    a = 2;
    {
        int b;
        b = 3;
        {
            int a;
            a = 5;
            int d;
            d = a + 2;
        }
        return b;
    }
    return a;
}