#include"TurboFS.h"

//initializing the Superblock and group descriptors
static superblock super;  //does not need mem alloc -> not a pointer
static bg_descriptor *groups_table; //allocated memory in sgd()


/*METADATA RELATED FUNCTIONS*/
int set_super_block()
{
	//needs to seek on disk(file)
	//read from disk
}


void set_group_descriptors()
{

	//1. Setup groups 
	number_of_groups = (super.s_blocks_count - 1)/(super.s_blocks_per_group) + 1;

	if(print_logs)
	{
		printf("Number of Groups : %d\n",number_of_groups );
	} 

	groups_table = (bg_descriptor*)malloc(sizeof(bg_descriptor) * number_of_groups);


	//try to print info on block groups : (refer structure)
	//but to access the groups_table-> skip super block -> make device seek by providing offset

	uint32 offset_of_super = size_of_block;
	if(size_of_block==1024)
		offset+=1024;

	//seek that offset on the disk

	//loop from 0 to no_of_groups and read groups table for each location



}






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
//Following the fuse.h file for any details !!!!!


void turbo_init(struct fuse_conn_info *conn)
{

	int status_returned = set_super_block();
    set_group_descriptors();

    //test();
    printf("\n\n\n\n");


}

int turbo_get_attr(const char *path, struct stat *stat_buffer) //is same as stat
{
	if(print_logs)
		printf("getattr: %s\n",path );

	//allocate mem for the stat pointer and fill with 0
	memset(stat_buffer,0,sizeof(struct stat));

	//path is a pointer and will be parsed , so make copy
	uint32 m = strlen(path);
	char temp_path[m+1];
	strncpy(temp_path,path,m); //needs to append null
	temp_path[m]=0;



	uint32 entry_inode_no = .......(path,ROOT_INODE); //start from the root and find this inode
	if(entry_inode_no ==0 )
	{
		printf("Entry Inode not found for %s\n",path );
		return;
	}


	inode * directory_inode = read_inode(entry_inode_no);

	//set up the stats(refer stat() manpage:

	stat_buffer->st_mode = directory_inode->i_mode;
	stat_buffer->st_uid = directory_inode->i_uid;
	stat_buffer->st_gid = directory_inode->i_gid;

	stat_buffer->st_size = directory_inode->i_size;
	statbuf->st_nlink = directory_inode->i_links_count;


	if(print_logs)
		printf("Successful get attributes\n");

	free(directory_inode);
	return 0;


}

int turbo_read_dir(const char *path, void *buf, fuse_fill_dir_t filler,
 						off_t offset,struct fuse_file_info *fileInfo)
{
	
	if(print_logs)
		printf("read_dir: %s\n",path );

	file_handle *fh = (file_handle *)fuse_file_info->fh;

	uint32 entry_inode_no = fh->f_inode;

	inode * directory_inode = read_inode(entry_inode_no);

	if(print_logs)
		printf("Dir Inode[0]:%lu\n"directory_inode->i_direct[0]);


	uint32 counter=0;
	uint32 counter_block;
	unsigned char block[size_of_block]; 

	if(read_inode_logic_block(block , *directory_inode , counter_block++)!=0)
		return -1;

	dir_ent * entry = (dir_ent *)block; //first entry

	while(counter < directory_inode->i_size)
	{
		if(counter > 0 && counter % size_of_block == 0)
		{
			read_inode_logic_block(block , *directory_inode , counter_block++);
		}

		char temp_file_name[MAX_NAME_LEN+1];
		memcpy(temp_file_name, entry->name , entry->name_len);
		temp_file_name[entry->name_len]=0;

		counter+=entry->rec_len;


		if(print_logs)
			printf("read_dir read: %u %s\n",entry->inode, temp_file_name);


		filler(buf,temp_file_name,NULL,0);

		//go to next entry for directory
		entry = (dir_ent*)(entry + entry->rec_len);


	}


	if(print_logs)
	{
		printf("%u of %u\n", directory_inode->i_size, counter);
	}

	return 0;

}


//phase 1:

int turbo_create()
{
	
}

int turbo_opendir(const char *path, struct fuse_file_info *fileInfo)
{

}


int turbo_open()
{

}

int turbo_read()
{

}











//phase 2

int turbo_mkdir()
{

}

int turbo_write()
{

}

int turbo_rename()
{

}

int turbo_unlink()
{

}







int turbo_stat()
{

}


int turbo_release()
{

}

int turbo_releasedir()
{

}

int turbo_truncate()
{

}

int turbo_rmdir()
{

}



