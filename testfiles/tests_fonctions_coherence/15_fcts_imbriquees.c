int add(int a, int b) { return a+b; }
int f(int a, int b, int c, int d, int e, int f, int g) { return add(a,g); }
int main() { return f(10,0,0,0,0,0,32); }
// attendu: 42