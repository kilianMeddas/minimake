#include <stdio.h>
#include <unistd.h>
int main(void)
{
printf("This line runs\n");
execlp("pwd", "## PROGRAM NAME ##", "--help", NULL);
printf("This line will not run, my process was replaced\n");
return 0;
}
