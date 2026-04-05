int test_loop(int n) {
    int i = 0;
    int res = 0;
    while (i < n) {
        if (i == 5) {
            return 100; // Sortie prématurée : la pile doit être nettoyée 
        }
        i = i + 1;
        res = res + i;
    }
    return res;
}