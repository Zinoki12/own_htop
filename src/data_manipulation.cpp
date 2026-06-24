#include "data_manipulation.hpp"
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#define BUFFER_SIZE 512
#define ll long long

const char *delimeters = " ";

struct processInfo {
  std::string la;
  std::string uptime;
  int pid;
  std::string User;
  short priority;
  short niceseness;
  char status;
  std::string virtual_memory;
  std::string res;
  int shared_mem;
  float cpu;
  float mem;
  std::string time;
  std::string command;
};

struct statInfo {
  char status;
  short pri;
  short ni;
};

struct statusInfo {
  int uid;
  ll virt;
  ll res;
  ll shr;
};

std::string read_uptime() {
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

  return result;
}

std::string load_average() {
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

  return line;
}

statusInfo parse_status(char file[]) {
  statusInfo status = {};
  std::ifstream fd(file);
  std::string line;

  std::stringstream ss;
  std::string prefix;
  ll rssfile = 0, rssshm = 0;

  if (fd.is_open()) {
    while (std::getline(fd, line)) {
      if (line.rfind("Uid:", 0) == 0) {
        ss.str(line);

        ss >> prefix >> status.uid;

        ss.clear();
      }

      else if (line.rfind("VmSize:", 0) == 0) {
        ss.str(line);

        ss >> prefix >> status.virt;

        ss.clear();
      }

      else if (line.rfind("VmRSS:", 0) == 0) {
        ss.str(line);

        ss >> prefix >> status.res;

        ss.clear();
      }

      if (line.rfind("RssFile:", 0) == 0) {
        ss.str(line);

        ss >> prefix >> rssfile;

        ss.clear();
      }

      if (line.rfind("RssShmem:", 0) == 0) {
        ss.str(line);

        ss >> prefix >> rssshm;

        ss.clear();
      }
    }
    status.shr = rssfile + rssshm;
  }

  return status;
}

// Доделать обновленную версию через структуру
statInfo stat_parse(char file[]) {
  file[strlen(file) - 2] = '\0';
  FILE *fp = fopen(file, "r");
  struct statInfo info;
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
        info.status = *word;
        ;
        break;
      case 17:
        info.pri = atoi(word);
        break;
      case 18:
        info.ni = atoi(word);
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
  return info;
}

//Доделать добавление в вектор данных

processInfo read_info() {
  DIR *dir;
  struct dirent *entry;
  std::vector<typename Tp>

  dir = opendir("/proc");

  if (!dir) {
    std::cerr << "Opendir error" << std::endl;
  }

  char name[BUFFER_SIZE];
  processInfo info = {};
  info.la = load_average();
  info.uptime = read_uptime();

  while ((entry = readdir(dir)) != NULL) {
    if (atoi(entry->d_name) != 0) {
      snprintf(name, sizeof(name), "/proc/%s/status", entry->d_name);
      statusInfo status = parse_status(
          name); // сделать тип данных и впихнуть сюда, фунция parse_status
      struct passwd *pw = getpwuid(status.uid);
      struct statInfo statInfo = stat_parse(name);
      info.pid = atoi(entry->d_name);
      info.User = pw->pw_name;
      info.priority = statInfo.pri;
      info.status = statInfo.status;
      info.niceseness = statInfo.ni;
    }

    // return NULL;
  }
}

// сделать вывод таблицы

void print_table(processInfo info){

}