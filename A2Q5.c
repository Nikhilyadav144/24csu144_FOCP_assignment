#include <stdio.h>
int main() {
int marks[] = {69, 90, 75, 55, 95};
int n = 5, sum = 0;
for (int i = 0; i < n; i++) {
sum += marks[i];
}
printf("Sum of all scores: %d", sum);
return 0;
}