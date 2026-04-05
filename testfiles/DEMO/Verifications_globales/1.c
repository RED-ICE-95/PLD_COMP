int main() {
    int a;
    a = 1;
    {
        int b;     // warning : b jamais utilisé
        {
            int c;
            c = a; 
            a = c + 1;
        }
    }
    return a;
}