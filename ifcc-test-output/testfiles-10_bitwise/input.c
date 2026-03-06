// Operateurs bit-a-bit : & prioritaire sur ^, ^ sur |
// 0b110 & 0b101 = 0b100 = 4
// 4 ^ 3 = 7
// 7 | 8 = 15
int main() {
    return 6 & 5 ^ 3 | 8;
}
