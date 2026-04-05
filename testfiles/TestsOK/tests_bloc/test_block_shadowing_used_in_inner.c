// shadowing : on utilise le a intérieur dans une expression
int main() {
    int a;
    a = 2;
    {
        int a;
        a = 3;
        int c;
        c = a;
    }
    return a;
}