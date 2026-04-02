// LIMITE : tableaux multi-dimensionnels non supportés
int main() {
    int mat[2][3] = {{1, 2, 3}, {4, 5, 6}};
    return mat[1][2]; // attendu : 6
}
