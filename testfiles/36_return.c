int main () {
    int a;        

    a = 2;       

    {
        int b;   
        b = 7;
        {
            int a;   
            a = 8;
            int d;
            d=a+2;    
        }  
    } 
    return a; 
}

//doit retourner 2