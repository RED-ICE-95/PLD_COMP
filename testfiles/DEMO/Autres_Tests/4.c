#include <stdio.h>

int main() {
    int choice = 0;
    int total = 0;
    int i = 0;
    while (i < 5) {  
        choice = i % 4; 
        i++;
        switch (choice) {
            case 0:
                total += 10;
                break;          
            case 1:
                total += 20;
                break;
            case 2:
                total += 30;
                continue;      
            default:
                total += 40;
        }
        if (total > 60) {
            break;  
        }
    }

    return total;  
}