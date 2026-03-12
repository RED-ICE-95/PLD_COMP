int main () {
    int a;        // scope global
    a = 1;        // doit utiliser a global

    {
    }

    {
    }

    return a;     // doit retourner a global = 1
}