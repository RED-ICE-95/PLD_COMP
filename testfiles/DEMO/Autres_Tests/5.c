int check_logic(int a, int b) {
    int valid = 0;
    // Utilisation de && et ||
    if ((a > 0 && b > 0) || (a < 0 && b < 0)) {
        valid = 1;
    }
    return valid;
}

int main() {
    int i = 0;
    int total = 0;
    int limit = 5;

    // Test Boucle + Incrémentation (++) + Affectation composée (+=)
    while (i < limit) {
        total += i; 
        i++;        
    }

    // Test Décrémentation (--) + Affectation composée (*=)
    int j = 3;
    int factor = 1;
    while (j > 0) {
        factor *= 2; 
        j--;         
    }

    // Test logique et shadowing
    if (check_logic(total, factor) && !(total == 0)) {
        int total = 42; 
        putchar(65); // 'A'
    } else {
        putchar(70); // 'F'
    }

    // Test -= et /=
    int final_val = 100;
    final_val -= 50; 
    final_val /= 5;  
    
    // Expression complexe 
    int magic = (final_val * 2 + (10 / 2)) & 255;
    
    if (magic > 15 || (magic < 5 && magic != 0)) {
        putchar(33); // '!'
    }

    putchar(10); 
    return total; 
}