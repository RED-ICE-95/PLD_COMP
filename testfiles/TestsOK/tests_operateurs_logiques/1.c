int main() {
    int a;
    int b;
    int res;

    a = 4;
    b = 2;
    res = 0;

    
    while ((a && b) || (a > 0)) {

        if ((a && (b - 1)) || (b && (a - 2))) {
            res = res + a;
        } else {
            res = res + b;
        }

        
        if (a && (10 / a)) {
            res = res + 1;
        }

        a = a - 1;
        b = b - 1;
    }

    return res;
}

// doit retourner 14