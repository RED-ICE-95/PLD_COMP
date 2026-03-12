int main () {
    int a;        

    a = 2;       

    {
        int b;   
        b = 2;
        {
            int a;   
            a = 5;
            int d;
            d=a+2;     
        }
    return a;    
    }

    return a;     
}