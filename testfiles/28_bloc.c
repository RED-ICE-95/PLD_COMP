int main () {
    int a;     
    a=2;   
    {
        int b;
        b=5;     
        a = b;   
    }

    return a;     
}