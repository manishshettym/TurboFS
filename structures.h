#define FUSE_USE_VERSION 31

 #include <fuse.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <stdbool.h>
 #include <string.h>
 #include <unistd.h>
 #include <fcntl.h>
 #include <sys/stat.h>
 #include <errno.h>
 #include <sys/time.h>



 typedef struct 
 {
    bool used;                  // valid inode or not
    int id;						// ID for the inode
    size_t size;				// Size of the file
    int data;					// offset of data block
    bool directory;				// true if its a directory else false
    int last_accessed;			// Last accessed time
    int last_modified;			// Last modified time
    int link_count; 			// 2 in case its a directory, 1 if its a file

 } __attribute__((packed, aligned(1))) inode;




 typedef struct 
 {
  int magic;
  size_t blocks;
  size_t iblocks;
  size_t inodes;
 } __attribute__((packed, aligned(1))) sblock;

 




 typedef struct
 {
  char filename[15];
  int file_inode;
 }dirent;

 // Size of dirent = 16 bytes
 // Hence in a 4KB block we can have 256 directory entries