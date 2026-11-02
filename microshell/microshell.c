#include <stddef.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Error: invalid number of arguments\n");
        return 1;
    }
    int pid = fork();
    if (pid == -1)
    {
        fprintf(stderr, "Error: fork failed\n");
        return 1;
    }

    if (pid == 0)
    {
        execl("/bin/sh", "supershell", "-c", argv[1], NULL);

        fprintf(stderr, "Error: exec failed\n");
        return 1;
    }

    int wstatus;
    int child_pid = waitpid(pid, &wstatus, 0);

    if (child_pid == -1)
    {
        fprintf(stderr, "Error: waitpid failed\n");
        return 1;
    }

    if (!WIFEXITED(wstatus))
    {
        fprintf(stderr, "Error: child did not exit normally\n");
        return 1;
    }
    printf("process exit status: %d\n", WEXITSTATUS(wstatus));

    return 0;
}
