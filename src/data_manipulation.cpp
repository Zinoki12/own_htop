#include "../header/data_manipulation.hpp"
#include <cstdio>
#include <pwd.h>

#define BUFFER_SIZE 512

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
  fclose(fp);

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

char *load_average() {
  FILE *fp = fopen("/proc/loadavg", "r");

  if (fp == NULL) {
    std::cerr << "uptime error" << std::endl;
    return NULL;
  }

  char line[BUFFER_SIZE];
  size_t size = sizeof(line);
  fgets(line, size, fp);
  fclose(fp);

  float a, b, c;
  int res = sscanf(line, "%f %f %f", &a, &b, &c);
  if (res != 3) {
    std::cerr << "load_average error" << std::endl;
  }

  snprintf(line, size, "Load average: %.2f %.2f %.2f", a, b, c);

  return strdup(line);
}

int read_uid(char *file) {
  FILE *fp = fopen(file, "r");
  if (fp == NULL) {
    std::cerr << "Open status file error" << std::endl;
  }

  char line[BUFFER_SIZE];
  while (fgets(line, sizeof(line), fp) != NULL) {
    if (strncmp(line, "Uid:", 4) == 0) {
      int uid;

      if (sscanf(line, "Uid: %d", &uid) == 1) {
        return uid;
      }
    }
  }
  return -1;
}

void load_body_htop() {
  DIR *dir;
  struct dirent *entry;

  dir = opendir("/proc");

  if (!dir) {
    std::cerr << "Opendir error" << std::endl;
  }

  char name[BUFFER_SIZE];
  char *ld = load_average();
  char *uptime = read_uptime();

  printf("\t%s\n\t%s\n", ld, uptime);
  printf("PID\tUSER\n");

  while ((entry = readdir(dir)) != NULL) {
    if (atoi(entry->d_name) != 0) {
      snprintf(name, sizeof(name), "/proc/%s/status", entry->d_name);
      int uid = read_uid(name);
      struct passwd *pw = getpwuid(uid);
      printf("%s\t%s\n", entry->d_name, pw->pw_name);
    }

    // return NULL;
  }
}