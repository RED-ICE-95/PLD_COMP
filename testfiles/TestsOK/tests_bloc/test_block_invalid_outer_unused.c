// le a extérieur n'est jamais utilisé → warning/erreur
int main() {
    int a;
    {
        int a;
        a = 3;
    }
    return 0;
}