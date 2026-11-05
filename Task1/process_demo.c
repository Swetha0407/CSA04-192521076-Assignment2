
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid;

    printf("Parent process started.\n");
    printf("Parent PID: %d\n", getpid());

    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return EXIT_FAILURE;
    }
    else if (pid == 0)
    {
        printf("\nChild process created.\n");
        printf("Child PID: %d\n", getpid());
        printf("Parent PID: %d\n", getppid());

        sleep(15);

        printf("Child process terminating.\n");
        exit(EXIT_SUCCESS);
    }
    else
    {
        printf("\nParent process details:\n");
        printf("Parent PID: %d\n", getpid());
        printf("Child PID: %d\n", pid);

        waitpid(pid, NULL, 0);

        printf("\nChild process completed.\n");
        printf("Parent process terminating.\n");
    }

    return EXIT_SUCCESS;
}
