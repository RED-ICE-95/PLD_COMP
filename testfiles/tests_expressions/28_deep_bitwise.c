// Bitwise profond : 6 niveaux
// (0xFF & 0x0F) | ((0x12 ^ 0x10) & 0x0F)
// = 0x0F | (0x02 & 0x0F) = 15 | 2 = 15
int main() {
    return (255 & 15) | ((18 ^ 16) & 15);
}
