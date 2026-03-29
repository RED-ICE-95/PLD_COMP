int main() {
    int x;
    int y;
    int z;
    int secret; 

    x = 10;
    y = 5;
    secret = 'A'; 

    z = !((x > y) & 1); 

    if (z != 1) {
        int i;
        i = 0;
        while (i < 3) {
            int secret; 
            secret = 'A';
            putchar(secret);
            i = i + 1;
        }
    }

    putchar(secret ^ 4); 

    return 0;
}