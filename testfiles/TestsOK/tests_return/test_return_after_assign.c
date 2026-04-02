// return après modification de variable dans bloc intérieur
int main() {
    int a;
    a = 1;
    {
        a = 99;
    }
    return a;
}
