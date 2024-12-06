#include <stdio.h>
int main() {
int marks[] = {69, 90, 75, 55, 95};
int n = 5, count = 0;
for (int i = 0; i < n; i++) {
if (marks[i] == 99) {
printf("99 found at index %d\n", i);
count++;
}
}
printf("Total students with 99: %d", count);
return 0;
}