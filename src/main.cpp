#include "../header/data_manipulation.hpp"
#include <iostream>
#include <sched.h>
#include <unistd.h>

int main() {
  /* pid_t pid = fork();
  int pipe_fd[2];
  int read_pipe, write_pipe;

  if (pipe(pipe_fd)) {
    std::cerr << "pipe error" << std::endl;
  }

  read_pipe = pipe_fd[0];
  write_pipe = pipe_fd[1];

  if (pid == -1) {
    std::cerr << "fork error" << std::endl;
  } else if (pid) {

  } else { // child
  } */

  char *ans = read_uptime();
  std::cout << ans << std::endl;

  return 0;
}