// Test array with variable index and assignment
int main() {
    int arr[5];
    int i = 0;
    while (i < 5) {
        arr[i] = i * 2;
        i = i + 1;
    }
    return arr[4];
}
