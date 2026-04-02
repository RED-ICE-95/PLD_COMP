// shadowing : le a intérieur vaut 3, le a extérieur reste 2
int main() {
    int a;
    a = 2;
    {
        int a;
        a = 3;
    }
    return a;
}