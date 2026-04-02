// Test array out-of-bounds (undefined behavior)
// 
// Ce test a un comportement indéfini et ne peut donc pas être comparé entre différentes architectures/compilateurs.
// - pour GCC x86, o c'est 0 qui sera retrouné (parce que la mémoire non initialisée se trouve être zéro)
// - pour MSP430 + mspsim simulator: la RAM est initialisée à 0xffff au démarrage
//      -> donc arr[2] lit 0xffff à la place -> masqué à 8 bits -> retourne 255

int main() {
    int arr[2] = {5, 10};
    int x = arr[2]; // out-of-bounds read → undefined behavior
    return x;
}
