// Test array out-of-bounds (should not crash, but undefined behavior)
int main() {
    int arr[2] = {5, 10};
    int x = arr[2]; // out-of-bounds read
    return x;
}
