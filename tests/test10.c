float q = 2.5 * 16;

void print_sth() {

}

int main() {
    printf("Hello world!");
    float qq = 3 + 7.5;
    printf(qq);
    printf(3 + 7 + q);

    printf(true);
    printf('a');

    int foo;
    int x = 14 * 48 / 4;
    printf(x);
    int y = -15;
    printf(y);
    int z = x + y - 4;
    printf(z);

    if (z < 10.0) {
        printf(x - y);
    } else if (z > 100) {
        printf(4/-20.0);
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