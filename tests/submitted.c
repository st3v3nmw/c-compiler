void handle_print(int x, int y, int i) { // handling functions
    x = x + i * i; // arithmetic expression
    y = y * i;
    if (x > y) { // relational expression
        printf("X is maximum"); // Token String
    } else {
        printf("Y is maximum");
    }
}

int main() {
    int value1 = 10;// Token identifier
    int value2 = 20;
    int intr;

    for (intr = 0; intr < 15; intr = intr+1) { // control structure
        // print
        handle_print(value1, value2, intr);
    }

    return 0;
}