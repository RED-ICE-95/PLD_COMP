// le bloc intérieur modifie la variable du bloc extérieur
int main() {
    int a;
    a = 2;
    {
        a = 5;
    }
    return a;
}