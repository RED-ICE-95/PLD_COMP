// Test array with nested if/else and assignments
int main() {
    int arr[3] = {1, 2, 3};
    int x = 0;
    int y = 5;
    if (arr[0] < arr[2]) {
        if (y > 2) {
            x = arr[1] + y;
        } else {
            x = arr[2] - arr[0];
        }
    } else {
        x = arr[0] * y;
    }
    return x;
}
