int main() {
    int a;        // scope global

    a = 1;        // doit utiliser a global

    {
        int b;    
        b = 2;
        a = b;   
    }

    {
        int a;    
        a = 10;  
    }

    return a;     // doit retourner a global = 1
}