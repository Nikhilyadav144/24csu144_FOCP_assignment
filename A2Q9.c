#include <stdio.h>
int main() {
int marks[] = {69, 90, 75, 55, 95};
int n = 5;
for (int i = 0; i < n; i++) {
if ((i == 0 || marks[i] >= marks[i - 1]) && (i == n - 1 || marks[i]
>= marks[i + 1])) {
printf("Peak element: %d", marks[i]);
break;
}
}
return 0;
}