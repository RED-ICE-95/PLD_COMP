// blocs vides imbriqués ne doivent pas poser de problème
int main() {
    int a;
    a = 2;
    {
        {
        }
    }
    return a;
}