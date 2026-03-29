// Test if/else with array
int main() {
    int arr[3] = {1, 2, 3};
    int x = 0;
    if (arr[1] > 1) {
        x = arr[2];
    } else {
        x = arr[0];
    }
    return x;
}
