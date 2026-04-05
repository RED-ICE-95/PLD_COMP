// a - b + c - d + e   (left-to-right associativity of +/-)
// = ((((10-3)+7)-2)+5) = 17
// Also tests: sub then add then sub then add
int main() {
    int a; int b; int c; int d; int e;
    a = 10; b = 3; c = 7; d = 2; e = 5;
    return a - b + c - d + e;
}
