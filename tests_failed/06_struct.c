// LIMITE : structures non supportées
struct Point {
    int x;
    int y;
};

int main() {
    struct Point p;
    p.x = 3;
    p.y = 4;
    return p.x + p.y; // attendu : 7
}
