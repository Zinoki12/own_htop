#ifndef DATA_MANIPULATION_HPP
#define DATA_MANIPULATION_HPP
#include "pwd.h"
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <iostream>
#include <sched.h>
#include <stdio.h>
#include <sys/types.h>

struct processInfo;
struct statInfo;
struct statusInfo;

std::string read_uptime();
std::string load_average();
processInfo read_info();
statusInfo parse_status(char file[]);
statInfo stat_parse(char file[]);

#endif