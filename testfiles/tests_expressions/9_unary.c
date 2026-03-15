// Unaire - et ! combines
// -(3) = -3, mais on retourne sur 8 bits donc 256-3=253... on evite
// !0 = 1, !1 = 0 => 1 + 0 = 1
int main() {
    return !0 + !1;
}
