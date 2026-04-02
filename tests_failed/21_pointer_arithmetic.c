// LIMITE : arithmétique de pointeurs non supportée
int main() {
    int arr[5] = {10, 20, 30, 40, 50};
    int *p = arr;
    p = p + 2;
    return *p; // attendu : 30
}
