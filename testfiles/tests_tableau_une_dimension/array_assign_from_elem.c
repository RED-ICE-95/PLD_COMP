// Test array with assignment to array element from another array element
int main() {
    int arr[3] = {1, 2, 3};
    arr[1] = arr[2];
    return arr[1];
}
