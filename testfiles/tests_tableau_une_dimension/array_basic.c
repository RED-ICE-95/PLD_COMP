// Test array declaration, initialization, assignment, and access
int main() {
    int arr[4];
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = arr[0] + arr[1];
    arr[3] = arr[2] * 2;
    return arr[3];
}
