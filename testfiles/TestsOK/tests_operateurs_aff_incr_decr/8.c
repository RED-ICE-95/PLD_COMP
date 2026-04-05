int main() {
    int a;
    int b;
    int c;

    a = 10;
    b = 5;
    c = 2;

    a += 3;   
    b *= 2;    
    c--;       

    a -= b;    
    b /= c;    
    a++;       
    c += a;   

    a %= c;    
    b--;       

    return a + b + c;  
}

//doit retourner 18