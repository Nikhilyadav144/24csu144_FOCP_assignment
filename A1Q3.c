#include<stdio.h>
int subtract(int a,int b) {
    b = ~b+1;
        while (b!= 0) {
            int borrow=(a&b);
            a=a^b;
            b=borrow<<1;
        }
        
        return a;
    }
int main()
{
    int a,b;
    printf("Enter first numbers: ");
    scanf("%d",&a);
    printf("Enter second numbers: ");
    scanf("%d",&b);
    printf("subtraction of two numbers is: %d\n",subtract(a,b));
    return 0;
}
