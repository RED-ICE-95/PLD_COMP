int main () {
    int a;        

    a = 2;       

    {
        int b;   
        b = 7;
        {
            int a;   
            a = 7;
            int d;
            d=a+2;  
            return d;   
        }   
    } 
}

// doit retourner 9, pas 2 ni 7