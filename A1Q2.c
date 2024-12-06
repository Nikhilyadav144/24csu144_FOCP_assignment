#include <stdio.h>

int hcf(int num1, int num2) {
    if (num2 == 0)
        return num1;
    return hcf(num2, num1 % num2);
}

int main() {
    int num1, num2;
    printf("Enter first integer: ");
    scanf("%d", &num1);
    printf("Enter second integer: ");
    scanf("%d", &num2);
    printf("HCF of %d and %d is %d\n", num1, num2, hcf(num1, num2));
    return 0;
}