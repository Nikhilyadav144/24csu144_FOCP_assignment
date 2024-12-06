#include <stdio.h>
int main() {
int arr[] = {14, 12, 27, 19, 32};
int size = 5;
int count = 0;
for (int i = 0; i < size; i++) {
int n = arr[i], isPrime = 1;
if (n <= 1) isPrime = 0;
for (int j = 2; j < n; j++) {
if (n % j == 0) {
isPrime = 0;
break;
}
}
if (isPrime) count++;
}
printf("%d\n", count);
return 0;
}