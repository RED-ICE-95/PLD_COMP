//test avec un bloc d'instruction dans un case
int main() {
    int x;
    x = 1;

    switch (x) {
        case 1:
            {
            x = x + 2;
            }
        case 2:
            x = 100;
    }

    return x;
}