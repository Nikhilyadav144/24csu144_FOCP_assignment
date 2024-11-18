#include <stdio.h>
#include <math.h>

int is_armstrong(int num) {
    int original_num, remainder, n = 0, result = 0, power;
    original_num = num;

    while (original_num != 0) {
        original_num /= 10;
        n++;
    }

    original_num = num;

    while (original_num != 0) {
        remainder = original_num % 10;
        power = pow(remainder, n);
        result += power;
        original_num /= 10;
    }

    if (result == num)
        return 1;
    else
        return 0;
}

int main() {
    int num;
    printf("Enter a number: ");
    scanf("%d", &num);

    if (is_armstrong(num))
        printf("%d is an Armstrong number", num);
    else
        printf("%d is not an Armstrong number", num);

    return 0;
}