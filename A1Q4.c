#include <stdio.h>
int main() {
//Using a third variable
int c = 50, d = 100, temp;
temp = c;
c = d;
d = temp;
printf("c = %d, d = %d\n", c, d);
//Without using a third variable (Arithmetic addition and
(subtraction)
int e = 50, f = 100;
e = e + f;
f = e - f;
e = e - f;
printf("\ne = %d, f = %d\n", e, f);
//Without using a third variable (Arithmetic multiplication and
(division)
int g = 50, h = 100;
g = g * h;
h = g / h;
g = g / h;
printf("g = %d, h = %d\n", g, h);
//Using bitwise XOR operator
int a = 50, b = 100;
a = a ^ b;
b = a ^ b;
a = a ^ b;
printf("a = %d, b = %d\n", a, b);
return 0;
}