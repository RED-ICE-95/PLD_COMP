// a & b ^ c | d
// & binds tighter than ^, ^ tighter than |
// = ((a&b) ^ c) | d
// = (12&10) ^ 6 | 3
// 12&10: 1100&1010 = 1000 = 8
// 8^6:   1000^0110 = 1110 = 14
// 14|3:  1110|0011 = 1111 = 15
int main() {
    int a; int b; int c; int d;
    a = 12; b = 10; c = 6; d = 3;
    return a & b ^ c | d;
}
