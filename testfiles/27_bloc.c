int main() {
    int a;        // scope global

    a = 1;        // doit utiliser a global

    {
        int b;    // bloc interne
        b = 2;
        a = b;   // doit modifier a global
    }

    {
        int a;    // shadowing : nouveau a dans ce bloc
        a = 10;  // modifie a interne au bloc
    }

    return a;     // doit retourner a global = 1
}