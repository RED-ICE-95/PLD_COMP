// return dans un bloc intérieur
int main() {
    int a;
    a = 2;
    {
        return 7;
    }
    return a;
}
