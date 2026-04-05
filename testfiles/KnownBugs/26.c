// En C, une affectation est une expression : (x = z) est valide dans un switch.
// or notre grammaire ne gère pas correctement les affectations comme expressions dans switch(expr).
int main() {
    int x = 1;
    int y = 0;
    int z = 2;

    switch ( x = z) {
        case 2:
            y = 10;
    }

    return y;
}