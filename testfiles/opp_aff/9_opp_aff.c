int main() {
    int x;
    int y;
    int z;

    x = 5;
    y = 10;
    z = 2;

    if (x > y) {        
        x += y;        
        y++;            
    } else {
        x -= y;
        y--;
    }

    while (x < 15) {     
        x += 2;           
        z--;              
    }

    x %= 5;              
    y *= 2;             

    return x + y + z;     
}
// doit retourner 10