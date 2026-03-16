// deux blocs séquentiels au même niveau, variables indépendantes
int main() {
    int a;
    a = 1;
    {
        int b;
        b = 2;
        a = b;
    }
    {
        int b;
        b = 3;
        a = a + b;
    }
    return a;
}