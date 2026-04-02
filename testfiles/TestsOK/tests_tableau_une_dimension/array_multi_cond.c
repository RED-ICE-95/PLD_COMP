// Test multiple arrays, assignments, and conditionals
int main() {
    int a[2] = {1, 2};
    int b[2] = {3, 4};
    int x = 0;
    if (a[0] + b[1] > 4) {
        x = a[1] * b[0];
    } else {
        x = a[0] - b[1];
    }
    b[1] = x;
    return b[1];
}
