#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <fuse.h>
#include <libgen.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>


#define ROOT_INO 2
#define ENTRY_DIR 2
#define ENTRY_FILE 1
#define ENTRY_BASE_SIZE 8

static const int print_logs = 0;
