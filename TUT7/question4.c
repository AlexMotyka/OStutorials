#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
  pid_t pid;
  int status;

  pid = fork();

  if (pid < 0) { // Fail
    status = -1;
    exit(1);
  }

  else if (pid == 0) { // Child
    execl("./process", NULL);
    _exit (EXIT_FAILURE);
  }

  else { // Parent
    sleep(5);
    kill(pid, SIGTSTP);
    sleep(10);
    kill(pid, SIGCONT);
    if (waitpid (pid, &status, 0) != pid)
      status = -1;
    return status;
  }
}
