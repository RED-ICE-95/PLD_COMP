// tableau passé en argument et modifié dans la fonction
// Vérifie que les modifications faites dans la fonction appelée
// sont visibles dans la fonction appelante (passage par référence implicite).
void remplir(int tab[], int n) {
    int i = 0;
    while (i < n) {
        tab[i] = i + 1;
        i++;
    }
}

int main() {
    int arr[5] = {0, 0, 0, 0, 0};
    remplir(arr, 5);
    return arr[0] + arr[1] + arr[2] + arr[3] + arr[4]; // attendu : 15
}
