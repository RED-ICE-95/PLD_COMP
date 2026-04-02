// LIMITE : switch avec expression constante comme case (non-littéral)
// Les case n'acceptent que des constantes littérales CONST [0-9]+.
// Les expressions constantes comme 2+3 ne sont pas acceptées en case.
int main() {
    int x = 5;
    switch (x) {
        case 2+3:
            return 1;
        default:
            return 0;
    }
}
