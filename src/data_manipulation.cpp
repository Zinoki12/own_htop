#include "data_manipulation.hpp"
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
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

int read_uid(char file[]) {
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

char **stat_parse(char file[]) {
  file[strlen(file) - 2] = '\0';
  FILE *fp = fopen(file, "r");
  char **data = (char **)malloc(3 * sizeof(char *));
  char *word = (char *)malloc(BUFFER_SIZE * sizeof(char));

  if (fp == NULL) {
    std::cerr << "Error open file pri_ni" << std::endl;
  }

  // getting status, priority and niceness
  short space = 0, i = 0;
  int ch;
  while ((ch = fgetc(fp)) != EOF) {
    if (ch == ' ' || ch == '\n') {
      word[i] = '\0';

      switch (space) {
      case 2:
        data[0] = strdup(word);
        break;
      case 17:
        data[1] = strdup(word);
        break;
      case 18:
        data[2] = strdup(word);
        break;
      }
      i = 0;
      space++;
    } else {
      if (i < BUFFER_SIZE - 2) {
        word[i++] = (char)ch;
      }
    }
  }
  return data;
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

  printf("\t\t\t%s\n\t\t\t%s\n\n", ld, uptime);
  printf(" PID\tUSER\t\tPRI\tNI\tS\n");

  while ((entry = readdir(dir)) != NULL) {
    if (atoi(entry->d_name) != 0) {
      snprintf(name, sizeof(name), "/proc/%s/status", entry->d_name);
      int uid = read_uid(name);
      struct passwd *pw = getpwuid(uid);
      char **s_pri_ni = stat_parse(name);
      printf(" %-5s  %-5s\t\t%-3s\t%-3s\t%-3s\n", entry->d_name, pw->pw_name,
             s_pri_ni[1], s_pri_ni[2], s_pri_ni[0]);
    }

    // return NULL;
  }
}