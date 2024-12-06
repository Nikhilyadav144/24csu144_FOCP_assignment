#include <stdio.h>
int main() {
int marks[] = {80, 90, 75, 55, 95};
int n = 5;
for (int i = 0; i < n; i++) {
marks[i] += 5;
}
for (int i = 0; i < n; i++) {
printf("%d ", marks[i]);
}
return 0;
}