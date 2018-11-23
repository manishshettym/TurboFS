 // Macros

 #define ceil_it(x, y) (((x) + (y) - 1) / (y))
 #define superblock_size 16
 #define block_size 4096

 #define N_INODES 100
 #define DBLKS_PER_INODE 1 //max 1 inode = 1 data block
 #define DBLKS (DBLKS_PER_INODE * N_INODES)

 #define ceil_it(x, y) (((x) + (y) - 1) / (y))
 #define DATA_MAP_BLKS ceil_it(DBLKS, block_size)
 #define INODE_BLKS ceil_it(N_INODES*sizeof(inode), block_size)

 #define INODE_MAP_BLKS ceil_it(DBLKS, block_size)

 #define FS_SIZE (INODE_MAP_BLKS + INODE_BLKS + DATA_MAP_BLKS + DBLKS) * block_size 
 /*total size of the file system
 = sizeof(block) * total_blocks*/


// to limit the complexity
 #define MAX_NO_OF_OPEN_FILES 10



 // File operations
 //static void *turbo_init(struct fuse_conn_info *conn, struct fuse_config *cfg);
 static int turbo_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi);
 static int turbo_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi);
 static int turbo_mkdir(const char *path, mode_t mode);
 static int turbo_rmdir(const char *path);
 //static int turbo_rename(const char *from, const char *to, unsigned int flags);
 //static int turbo_truncate(const char *path, off_t size, struct fuse_file_info *fi);
 static int turbo_create(const char *path, mode_t mode, struct fuse_file_info *fi);
 static int turbo_open(const char *path, struct fuse_file_info *fi);
 static int turbo_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi);
 static int turbo_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi);
 static int turbo_rm(const char *path);