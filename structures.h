#include<stdint.h>
#define uint32 unsigned int
#define uint16 unsigned short int
#define uint8 uint8_t

#define MAX_NAME_LEN 255
#define NO_OF_DIRECT_BLOCKS 12


typedef struct t_superblock
{
	uint32 s_inodes_count; //inode count
	uint32 s_blocks_count; //no of blocks
	uint32 s_free_block_count; // free blocks
	uint32 s_free_inodes_count; // free inodes
	uint32 s_first_data_block; 
	uint32 s_log_block_size; //block size=4kb

	uint32 s_blocks_per_group; //reqd to set no of groups
	uint32 s_frags_per_group;
	uint32 s_inodes_per_group;
	uint32 s_mtime;
	uint32 s_wtime;
	

	uint16 s_mnt_count;

	
	uint16 s_magic;

	//figure out
	uint16 s_state;
	uint16 s_errors;
	uint16 s_minor_rev_level; 
	uint32 s_lastcheck;
	uint32 s_checkinterval;
	uint32 s_creator_os;
	uint32 s_rev_level;

	uint16 s_def_resuid;
	uint16 s_def_resgid;

	uint32 s_first_inode;
	uint16 s_inode_size;
	uint16 s_block_group_nr;

	//figue out
	uint32 s_feature_compat;
	uint32 s_feature_incompat;
	uint32 s_feature_ro_compat;
	uint16 s_uuid;
	uint16 s_pad;
	uint32 s_reserved[229];


}superblock;


typedef struct t_inode
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

	
	uint16 i_links_count; //count of links (st_nlink)
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

}inode;

typedef struct t_block_group_descriptor{
	uint32 bg_block_bitmap;
	uint32 bg_inode_bitmap;
	uint32 bg_inode_table;
	uint16 bg_free_blocks_count;
	uint16 bg_free_inodes_count;
	uint16 bg_used_dirs_count;
	uint16 bg_pad;
	uint32 bg_reserved1;
	uint32 bg_reserved2;
	uint32 bg_reserved3;
}bg_descriptor;


typedef struct t_dir_entry
{
	uint32	inode;			/* Inode number */
	uint16	rec_len;		/* Directory entry length */
	uint8	name_len;		/* Name length */
	uint8	file_type;
	char	name[MAX_NAME_LEN];	/* File name */
	uint32 block_number;
	uint32 offset;
}dir_ent;


typedef struct t_file_handle
{
	uint32 f_inode;
	uint32 f_size;
	uint32 f_blocks_count;
}file_handle;



