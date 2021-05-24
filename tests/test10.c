float q = 37/-100.0; // -0.37

int main() {
    printf("\nHello world!");
    
    if (53 > 89) { // false
        printf(q); // -0.37
    } else {
        printf(1 + q); // 0.63
    }

    int i = 0;
    while (i <= 3) {
        printf(i);
        i = i + 1;
    }

    int foo;
    for (foo = 32; foo > 29; foo = foo - 1) {
        printf(foo);
    }

    return 0;
}