int sum_arrays(int a[2], int b[2]) {
    int res = 0;
    res = a[0] + b[1]; // Test rmem + addition
    return res;
}

int main() {
    int t1[2] = {10, 20}; // Test visitDeclar avec initialisation
    int t2[2];
    t2[0] = 5;            // Test visitAssignArray (wmem)
    t2[1] = 100;

    int total = sum_arrays(t1, t2); // Test visitExprFonctCall avec tableaux

    return total; // Devrait retourner 110 (t1[0] + t2[1])
}