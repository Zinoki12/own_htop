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

struct file_data;

char *read_uptime();
char *load_average();
void load_body_htop();
int read_uid(char *file);

#endif