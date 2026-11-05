#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
int main(void)
{
int pid = fork();
if (pid == 0)
{
// child process
sleep(3);
exit(42);
}
// parent process
int wstatus;
printf("Waiting for child %d to end...\n", pid);
int child_pid = waitpid(pid, &wstatus, 0);
printf("Child process %d terminated.\n", child_pid);
printf("Process exit status: %d\n", WEXITSTATUS(wstatus));
return 0;
}
