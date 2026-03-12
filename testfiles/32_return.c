int main () {
    int a;        

    a = 2;       

    {
        int b;   
        b = 7;
        return b;
        {
            int a;   
            a = 5;
            int d;
            d=a+2;  
            return d;   
        }
    return a;    
    }
    return a;     
}

// doit retourner 7, pas 5 ni 9 ni 2