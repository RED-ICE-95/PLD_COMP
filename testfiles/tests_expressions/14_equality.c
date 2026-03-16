// Egalite et inegalite : priorite plus faible que les arithmetiques
// 1+2 == 3 doit etre (1+2)==3 = 1, pas 1+(2==3) = 1+0 = 1 (meme resultat ici)
// 2*3 != 7 = 1
// 1 + 1 = 2
int main() {
    return (1 + 2 == 3) + (2 * 3 != 7);
}
