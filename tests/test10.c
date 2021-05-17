int q = 2;

void print_sth() {

}

int main() {
    printf("Hello world!");
    float qq = 3 + 7.5;
    printf(qq);
    printf(3 + 7);

    printf(true);
    printf('a');

    int foo;
    int x = 14 * 48 / 4;
    printf(x);
    int y = -15;
    printf(y);
    int z = x + y - 4 * q;
    printf(z);

    if (z < 10) {
        printf(x - y);
    } else if (z > 100) {
        printf(1232);
    }

    int i = 0;
    while (i <= 5) {
        if (i >= 4) {
            printf(i);
        } else {
            printf(2);
        }
        i = i + 1;
    }

    for (foo = 0; foo < 15; foo = foo+1) {
        printf(foo);
    }

    return 0;
}