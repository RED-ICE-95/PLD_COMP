int main() {
    int x;
    int y;

    x = 64;
    y = 14;

    while (x > 1) {
        x /= y;   
        y /= 2;   
    }

    return x + y;   
}
// doit retourner 3