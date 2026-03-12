int main () {
    int a;     
    a=2;   
    {
        int a;
        a=3;
        int c;     
        c=a;
        
    }

    return a;     // doit retourner a global = 2
}