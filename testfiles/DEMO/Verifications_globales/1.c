int main() {
    int a;
    a = 1;
    {
        int b;     // warning : b jamais utilisé
        {
            int c;
            c = a; // c utilisé, a utilisé
            a = c + 1;
        }
    }
    return a;
}