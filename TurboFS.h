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

#include "structures.h"

#define ROOT_INODE 2
#define ENTRY_DIR 2
#define ENTRY_FILE 1
#define ENTRY_BASE_SIZE 8

static const int print_logs = 1;




static uint32 size_of_block;
static uint32 size_of_fragment;
static uint32 number_of_groups;
static uint32 n_512k_blocks_per_group;

static uint32 indirect_blocks_count;
static uint32 d_indirect_blocks_count;
static uint32 t_indirect_blocks_count;