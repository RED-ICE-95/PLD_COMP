
// Fonction récursive
int fibo(int n) {
    if (n < 2) { 
        return n;
    }
    return fibo(n - 1) + fibo(n - 2);
}

// Fonction pour tester opérations bit-à-bit et le shadowing
void check_parity(int n) {
    int is_even; 
    is_even = !(n & 1); 

    if (is_even) {
        int n; 
        n = 69; // 'E' en ASCII
        putchar(n); 
    } else {
        putchar(79); // 'O' en ASCII 
    }
}

int main() {
    int input;
    int result;
    int i = 0;

    input = 7; 

    while (i < input) {
        int current_fibo; 
        current_fibo = fibo(i);
        
        check_parity(current_fibo);
        
        i = i + 1;
    }

    putchar(10); 
    
    result = -(10 * 2) / (5 + 5); 
    if (!(result != -2)) { 
        putchar(33); 
    }

    return 0;
}