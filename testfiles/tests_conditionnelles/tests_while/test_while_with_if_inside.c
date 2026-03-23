// if à l'intérieur du while
int main() {
    int i;
    int result;
    i = 0;
    result = 0;
    while (i < 10) {
        if (i == 5) {
            result = 1;
        }
        i = i + 1;
    }
    return result;
}