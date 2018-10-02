#include<stdint.h>
#define uint32 unsigned int
#define uint16 unsigned short int
#define uint8 uint8_t

#define MAX_NAME_LEN 255
#define NO_OF_DIRECT_BLOCKS 12


struct t_inode
{
	uint16 i_mode;// represents whether directory OR file
	uint16 i_uid;
	uint16 i_gid;

	uint32 i_size; //size of file

	//times:
	uint32 i_atime; //arrival
	uint32 i_ctime; //created
	uint32 i_mtime; //modified
	uint32 i_dtime; //deleted

	
	uint16 i_links_count; //count of links
	uint32 i_blocks; //count of blocks
	uint32 i_flags; //flags for the inode

	uint32 i_direct[NO_OF_DIRECT_BLOCKS]; // allocated array for direct pointers
	uint32 i_indirect; //no of single indirect
	uint32 i_double_indirect; //no of double indirect
	uint32 i_triple_indirect; // no of triple indirect

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