// Test array with while and if inside loop
int main() {
    int arr[4] = {1, 2, 3, 4};
    int i = 0;
    int sum = 0;
    while (i < 4) {
        if (arr[i] % 2 == 0) {
            sum = sum + arr[i];
        }
        i = i + 1;
    }
    return sum;
}
