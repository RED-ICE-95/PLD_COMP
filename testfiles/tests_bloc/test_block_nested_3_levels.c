// 3 niveaux d'imbrication et  variable du plus haut niveau utilisée partout
int main() {
    int a;
    a = 2;
    {
        {
            {
                int d;
                d = a + 5;
            }
        }
    }
    return a;
}