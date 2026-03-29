
int calculate_magic(int n) {
    int result;
    
    if (n == 0) {
        return 1;
    }
    
    result = (n * 2) / 2;
    return result + calculate_magic(n - 1);
}

void process_display(int val) {
    int is_even;
    is_even = (val & 1) == 0;

    if (is_even) {
        int val = 69;
        putchar(val);
    } else {
        putchar(79); // 'O' 
    }
}

int main() {
    int limit;
    int i;
    int total;

    limit = 5;
    i = 0;
    total = 0;

    while (i < limit) {
        int current; 
        
        current = calculate_magic(i);
        process_display(current);
        
        total = total + current;
        i = i + 1;
    }

    putchar(10); 

    if (!(total == 0)) {
        int final_char;
        final_char = 'A' ^ 1; 
        putchar(final_char); 
    }

    return 0;
}