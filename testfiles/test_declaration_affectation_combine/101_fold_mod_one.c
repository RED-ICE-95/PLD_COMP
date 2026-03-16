// x%1 = 0 toujours (C99 garantit troncation vers zero)
// Attendu : 0
int main() {
    int x = 17;
    int a = x % 1;
    return a;
}
