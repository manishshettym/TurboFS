#include<stdint.h>
#define uint32 unsigned int
#define uint16 unsigned short int

//need to define preset values for known inodes
//root
//bad inode
//boot loader
//etc ..


int is_directory(uint16 inode_mode);

int is_file(uint16 inode_mode);
