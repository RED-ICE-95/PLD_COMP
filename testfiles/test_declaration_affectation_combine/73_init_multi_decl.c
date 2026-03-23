// Plusieurs vars sur une meme ligne, certaines init certaines non
// int x = 5, y, z = 10;  y = x + z; return y;
// Attendu : 15
int main() {
    int x = 5, y, z = 10;
    y = x + z;
    return y;
}
