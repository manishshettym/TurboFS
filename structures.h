#include<stdint.h>
#define uint32 unsigned int
#define uint16 unsigned short int
#define uint8 uint8_t

#define MAX_NAME_LEN 255
#define NDIRECT_BLOCKS 12


struct t_inode
{
	uint16 i_mode;// represents whether directory OR file
	uint16 i_uid;
	uint16 i_gid;

	uint32 i_size; //size of file

	//times:
	uint32 i_atime;
	uint32 i_ctime;
	uint32 i_mtime;
	uint32 i_dtime;

	
	uint16 i_links_count;
	uint32 i_blocks;
	uint32 i_flags;

	uint32 i_direct[NDIRECT_BLOCKS];
	uint32 i_indirect;
	uint32 i_double_indirect;
	uint32 i_triple_indirect;

	/*
	uint32 i_osd1;
	uint32 i_generation;
	uint32 i_file_acl;
	uint32 i_dir_acl;
	uint32 i_faddr;
	uint32 i_osd2_1;
	uint32 i_osd2_2;
	uint32 i_osd2_3;*/

}