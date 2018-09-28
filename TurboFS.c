#include"TurboFS.h"

/*METADATA RELATED FUNCTIONS*/




/*INODE RELATED FUNCTIONS*/

void read_inode_bitmap(void *buffer , int group_no)
{
	//get block number

	//read the block and return as pointer
}

void inode_bitmap_set(uint32 inode_no , uint8 state)
{
	//find the inode bitmap

	//set the bit if unset

	//else reset

	//if set -> reduce the free inodes available

	//else -> increase the free inodes available


	//writeback

}


struct t_inode *read_inode(uint32 inode_no)
{
	//locate the inode group and index 

	//allocate temp memory and seek the inode

	//copy to the temp inode var


	//return inode
}




/*BLOCK RELATED FUNCTIONS*/





/*FUSE FUNCTIONS*/


void turbo_init()
{

}

int turbo_get_attr()
{

}

int turbo_read_dir()
{

}

int turbo_opendir()
{

}

int turbo_stat()
{

}

int turbo_open()
{

}

int turbo_read()
{

}

int turbo_release()
{

}

int turbo_releasedir()
{

}

int turbo_mkdir()
{

}

int turbo_truncate()
{

}

int turbo_write()
{

}

int turbo_rename()
{

}


int turbo_rmdir()
{

}

int turbo_unlink()
{

}

int turbo_create()
{
	
}