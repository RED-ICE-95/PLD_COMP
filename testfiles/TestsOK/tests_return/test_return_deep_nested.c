// return depuis un bloc très imbriqué
int main() {
    int a;
    a = 1;
    {
        {
            {
                return 42;
            }
        }
    }
    return a;
}
