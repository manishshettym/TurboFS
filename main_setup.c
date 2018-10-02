#include"TurboFS.h"
#include<fuse.h>
#include<stdio.h>


//defining the fuse structure to be initialized

struct fuse_operations tfs_oper;


int main(int argc, char *argv[])
{
	
	//initialize all fuse operations
	tfs_oper.init = turbo_init;

	tfs_oper.getattr = turbo_get_attr;

	tfs_oper.readdir = turbo_read_dir;
	tfs_oper.opendir = turbo_opendir;

	tfs_oper.statfs = turbo_stat;
	tfs_oper.open = turbo_open;
	tfs_oper.read = turbo_read;
	tfs_oper.write = turbo_write;

	tfs_oper.mkdir = turbo_mkdir;
	tfs_oper.rmdir = turbo_rmdir;
	tfs_oper.release = turbo_release;
	tfs_oper.releasedir = turbo_releasedir;
	
	tfs_oper.rename = turbo_rename;
	tfs_oper.truncate = turbo_truncate;
	tfs_oper.unlink = turbo_unlink;
	tfs_oper.mknod = turbo_mknod;
	tfs_oper.create = turbo_create;
	tfs_oper.utime = turbo_utime;


	//MOUNTING THE FS
	//...





	int status_of_fuse = fuse_main(argc,argv,&tfs_oper,NULL);
	printf("fuse_main returned status:%d\n",status_of_fuse );

	return status_of_fuse;


}
