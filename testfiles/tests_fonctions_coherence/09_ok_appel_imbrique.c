// Appel imbriqué correct
int f(int x) { return x+1; }
int g(int y) { return f(y)+1; }
int main() { return g(3); }