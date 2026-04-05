// GCC : retourne 30 —> le shadowing fonctionne correctement, chaque 'i' est
//       résolu dans son scope, le 'i' de la boucle est bien incrémenté.
// IFCC : boucle infinie —> notre mécanisme de scopeRename résout 'i' dans
//        le scope le plus récent. Quand on fait "i = i + 1" à la fin du
//        while, resolve("i") trouve le 'i' du bloc intermédiaire (i=10)
//        au lieu du 'i' de la boucle. Le 'i' de la boucle n'est donc
//        jamais incrémenté.

/*int main() {
    int i = 0;
    int res = 0;
    while (i < 2) {
        int i = 10; // Ce 'i' (10) cache l'index de boucle
        {
            int i = 5; // Ce 'i' (5) cache le 'i' (10)
            res = res + i;
        }
        res = res + i;
        i = i + 1; 
    }
    return res; // Attendu : (5 + 10) + (5 + 10) = 30
}*/

// Boucle infinie 