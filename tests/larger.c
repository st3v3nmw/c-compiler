// function
bool f(float e, float p) {
    return (e * e * p) != 1.0;
}

int main() {
    // declarations
    float p;
    int r;

    // initializations
    bool q = false;
    float e = 2.71828;
    p = -314.159265;
    r = 42;
    int t = -53;
    char c = "e";

    // "assignments"
    t += 1;
    r--;
    p /= 10;
    p *= e;
    float z = (e * e / 2.0) * p;

    // if
    if ((f(e, p) && r == 41) || r == 42) {
        if (c == "u") {
            printf("yay\n");
        } else {
            printf("nay\n");
        }
    }

    // loops
    for (int i = 0; i * i <= 10; i++) {
        printf("cogito ergo sum\n");
    }

    int i = 0;
    while (true) {
        i++;
        if (i * i > 10) {
            printf("hmm too much\n");
            break;
        }
        printf("cogito ergo sum\n");
    }
    return 0;
}