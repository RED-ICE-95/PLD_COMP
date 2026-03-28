int main() {
    int x;
    int y;
    int z;

    x = 5;
    y = 10;
    z = 2;

    x += 3;       // test +=
    y -= 4;       // test -=
    z *= x;       // test *=
    x /= 2;       // test /=
    y %= 3;       // test %=

    x++;          // test ++
    z--;          // test --

    if (x > y) {
        y += z;
    } else {
        x += z;
    }

    while (x < 20) {
        x++;
        z += 1;
    }

    return x + y + z;
}
// doit retourner 65