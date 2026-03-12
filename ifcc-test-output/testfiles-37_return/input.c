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
        return b; 
    } 
}

//doit retourner 7