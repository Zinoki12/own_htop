#include "../header/data_manipulation.hpp"
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sched.h>
#include <stdio.h>

#define BUFFER_SIZE 256

const char *delimeters = " ";

struct file_data {
  pid_t pid;
  char user[BUFFER_SIZE];
};

char *read_uptime() {
  FILE *fp = fopen("/proc/uptime", "r");

  if (fp == NULL) {
    std::cerr << "uptime error" << std::endl;
    return NULL;
  }
  char buffer[BUFFER_SIZE];

  fgets(buffer, sizeof(buffer), fp);

  char *uptime = strtok(buffer, delimeters);

  int time = atoi(uptime);
  int days = time / 86400;
  short hours = (time % 86400) / 3600;
  short minutes = ((time % 86400) % 3600) / 60;
  short seconds = (((time % 86400) % 3600) % 60) % 60;

  char result[BUFFER_SIZE];
  size_t size = sizeof(result);
  if (days > 0) {
    snprintf(result, size, "Uptime: %d days, %d:%d:%d", days, hours, minutes,
             seconds);
  } else {
    snprintf(result, size, "Uptime: %d:%d:%d", hours, minutes, seconds);
  }

  return strdup(result);
}