#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>

// gcc main.c -o main

int main(int argc, char *argv[])
{
    int pid = fork();
    if (pid == -1)
    {
        return 1;
    }
    if (pid == 0)
    {
        // child process
        int file = open("pingResults.txt", O_WRONLY | O_CREAT, 0777);
        if (file == -1)
        {
            return 2;
        }
        // int file2 = dup(file);
        printf("fd to pingResults %d\n", file);
        int file2 = dup2(file, STDOUT_FILENO);
        printf("The duplicated fd: %d\n", file2);
        close(file);

        int err = execlp("ping", "ping", "-c", "3", "google.com", NULL);
        if (err == -1)
        {
            printf("Could not fuind program to execute! \n");
            return 0;
        }
    }
    else
    {
        // Parent process
        int wstatus;
        wait(&wstatus);
        if (WIFEXITED(wstatus))
        {
            int statuseCode = WEXITSTATUS(wstatus);
            if (statuseCode == 0)
            {
                printf("\nSuccess!\n");
            }
            else
            {
                printf("\nfailure !\n");
            }
        }

        printf("\nSome post processing goes here!!\n");
    }

    return 0;
}