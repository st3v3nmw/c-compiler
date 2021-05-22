float q = 37/-100.0; // -0.37

int main() {
    printf("\nHello world!");
    printf(1 + q); // 0.63

    printf("\nIf output:");
    if (78 > 10) { // false
        printf("q > 10 and 2 / 3 < 1");
    } else {
        printf("q < 10 or 2 / 3 > 1");
    }

    printf("\nWhile loop output:");
    int i = 0;
    while (i <= 3) {
        printf(i);
        i = i + 1;
    }

    printf("\nFor loop output:");
    int foo;
    for (foo = 14; foo > 10; foo = foo - 1) {
        printf(foo);
    }

    return 0;
}