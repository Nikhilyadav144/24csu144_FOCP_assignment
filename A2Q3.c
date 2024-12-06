#include <stdio.h>
int main() {
int marks[] = {69, 90, 75, 55, 95};
int n = 5;
for (int i = 0; i < n; i++) {
if (marks[i] == 99) {
printf("First 99 found at index %d", i);
break;
}
}
return 0;
}