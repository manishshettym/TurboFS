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

#include "structures.h"
#include "turbofs.h"


#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"
#define BOLD  "\x1b[1m"


 static struct fuse_operations turbo_oper = 
 {
  
  .getattr    = turbo_getattr,
  .readdir    = turbo_readdir,
  .mkdir      = turbo_mkdir,
  .rmdir      = turbo_rmdir,
  .open       = turbo_open,
  .create     = turbo_create,
  .read       = turbo_read,
  .write      = turbo_write,
  .unlink     = turbo_rm,

 };


 #define DEBUG


 //Inode related Functions
 int initialise_inodes(int* i);
 int initialise_data_map(int* map);
 int return_first_unused_inode(int* i);
 int return_offset_of_first_free_datablock(int *data_map);
 int isDir(char *path);
 void path_to_inode(const char* path, int *ino);
 void allocate_inode(char *path, int *ino, bool dir);
 void print_inode(inode *i);

 int turbo_file;
 char *fs;// The start of the FileSystem in the memory

 int *inode_map;// inode bitmap
 inode *inodes;// The start of the inode block

 int *data_map;//The start of the databitmap block
 char *datablks;//The start of the data_blockss

 dirent *root_directory; // Address of the block representing the root directory

    

int main(int argc, char *argv[])
{

  //PERSISTENCE STORAGE
  turbo_file = open("TurboFileSystem", O_RDWR);
  struct stat buf;
  fstat(turbo_file, &buf);


  #ifdef DEBUG
    printf( BLU BOLD "TurboFS secondary disk size = %d\n" RESET, buf.st_size); 
  #endif

    printf(GRN BOLD "Superblock size = %d\n" RESET, superblock_size);
    printf(GRN BOLD "Block size = %d\n\n", block_size);

    printf(GRN BOLD "Total no of inodes = %d\n" RESET,N_INODES);
    printf(GRN BOLD "Size of inode = %d\n" RESET,sizeof(inode));
    printf(GRN BOLD "Inode Blocks = %d\n" RESET, INODE_BLKS);
    printf(GRN BOLD "Inode_Bitmap size(blocks)= %d\n"RESET,INODE_MAP_BLKS);
    printf(GRN BOLD "Total no of datablocks= %d\n" RESET, DBLKS);
    printf(GRN BOLD "Data_Bitmap size(blocks) = %d\n\n" RESET,DATA_MAP_BLKS);



    printf(BLU BOLD "Total File System Size = %d\n\n\n" RESET,FS_SIZE);

    


  fs = calloc(1, FS_SIZE);

  if(buf.st_size != 0)
  {
    read(turbo_file, fs, FS_SIZE);
  }

  //allocating the right memory
  inode_map = (int *)fs;
  inodes = (inode *)(inode_map + INODE_BLKS * block_size);
  data_map = (int *)(inodes + N_INODES * sizeof(inode));
  datablks = (char *)(data_map + DATA_MAP_BLKS * block_size);

  //logging the starting addresses

  printf("Starting addresses .... \n\n\n");

  printf("fs = %u\n", fs);
  printf("inode_map = %u\n", inode_map);
  printf("inodes = %u\n", inodes);
  printf("data_map = %u\n", data_map);
  printf("datablks = %u\n", datablks);

  if(buf.st_size == 0)
  {
    initialise_inodes(inode_map);
    initialise_data_map(data_map);
  }

  printf("TurboFS initializing ... ...\n\n");


	// Initialising the root directory
	root_directory = (dirent *)datablks;

	printf("root_directory = %u\n", root_directory);

  return fuse_main(argc, argv, &turbo_oper, NULL);
}




int initialise_inodes(int* i)
{
	// Initalise the inodes
	// return 0 on success and -1 on some error
	for(int x = 0; x < N_INODES; x++)
  {
		*i = 0;
		i++;
	}
	return 0;
}


//data bit map for each datablock
//last block has -1 indicating it is the end( no more free blocks)
int initialise_data_map(int* map){
	// Initalise the data_map
	int x;
	for(x = 0; x < DBLKS; x++)
  {
		*(map + x) = 1;
	}
	*(map + x) = -1; // No more datablocks
	return 0;
}


//free inode function to search the inode bitmap
//Todo : implement better search if possible

int return_first_unused_inode(int* i)
{
	int ix;
	for(ix = 1; ix < N_INODES; ix++,i++)
  {
		if(*i == 0)
    {
			*i = 1;
			return ix;
		}
	}
	return -1;
}

//free data block function to search the data bitmap

int return_offset_of_first_free_datablock(int *data_map)
{
	int i;
	for(i = 1; i < DBLKS; i++)
  {
		//printf("%d\n", data_map[i]);
		if(data_map[i] == 1)
    {
			data_map[i] = 0; //mark it as used now
			return (i);
		}
	}
	return -1;
}


//parse the path to reach the correct inode
//use the directory entries to parse file names and finally get the inode
//to which it is pointing

void path_to_inode(const char* path, int *ino)
{
	// Given the path name it will return the inode address if it exists else it returns NULL

  //if path itself is root -> already have pointer to the root directory
  //so dont search further
	if(strcmp("/", path) == 0)
  {
		*ino = (root_directory->file_inode);
	}

  //if not root -> start at root and search further
	else
  {
  		char *token = strtok(path, "/");
  		dirent *temp;
  		temp = root_directory;

      while (token != NULL)
      {
          //no more files in directory AND file not found
    			while((strcmp(temp -> filename, "") != 0) && (strcmp(temp -> filename, token) != 0))
          { 
    				temp++;
    			}

        //if reached end of the path and temp is empty => NOT FOUND
  			if(strcmp(temp -> filename, "") == 0)
        {
  				//printf("Cannot find Inode @%s\n", path);
  				*ino = -1;
  			}


  			else
        {
          printf( MAG BOLD "INODE has been FOUND !!! \n\n" RESET);

  				*ino = (temp -> file_inode);
          inode *temp_ino = inodes + ((*ino) * sizeof(inode));
  				if(temp_ino -> directory)
          {
  					temp = (dirent *)(datablks + ((temp_ino -> data) * block_size));
  				}

  			}

  			token = strtok(NULL, "/");
  		}


	}


}


//do same as above and run through the path until you find the last "/" 
//then check the  dirent for pointer to -> inode
//from inode -> check the bool variable 

int isDir(char *path)
{
  //if root => directory
	if (strcmp("/", path) == 0)
  {
		return 1;
	}

	else
  {
		char *token = strtok(path, "/");
		dirent *temp;
		temp = root_directory;
	
		int dir = -1;
    while (token != NULL)
    {
    	while(((strcmp(temp -> filename, "") != 0) || (temp -> file_inode) != 0))
      {
				if((strcmp(temp -> filename, "") != 0))
        {
					if((strcmp(temp -> filename, token) == 0))
          {
						break;
					}
				}
				temp ++;

			}

			if((strcmp(temp -> filename, "") == 0) && temp -> file_inode == 0)
      {
				return -1;
				// Invalid path
			}

			else
      {
        inode *temp_ino = inodes + ((temp -> file_inode) * sizeof(inode));

        //if bool is true
				if(temp_ino -> directory)
        {
					dir = 1;
				}
				else
        {
					dir = 0;
				}

				if(dir == 1){
					temp = (dirent *)(datablks + ((temp_ino -> data) * block_size));
				}
				// not handled : case where file/directory is there

			}
      token = strtok(NULL, "/");
  	}


	
		if(dir == 0)
    {
			printf("%s - FILE\n", path);
		}
		else if(dir == 1)
    {
			printf("%s - DIRECTORY\n", path);
		}
		
  return dir;
	}
}

//set the parameters for the free location found in inode bit map
void allocate_inode(char *path, int *ino, bool dir)
{
	//*ino = return_first_unused_inode(inodes);

  //position the pointer to correct address
  inode *temp_ino = inodes + ((*ino) * sizeof(inode));

	temp_ino -> id = rand() % 5000;
	temp_ino -> size = 0;
	temp_ino -> data = return_offset_of_first_free_datablock(data_map);
	temp_ino -> directory = dir;
	temp_ino -> last_accessed = 0;
	temp_ino -> last_modified = 0;

  if(dir)
  {
    temp_ino -> link_count = 2;
  }
  else
  {
    temp_ino -> link_count = 1;
  }

}


//print inode related things
void print_inode(inode *i)
{
  printf(RED BOLD);
	printf("used : %d\n", i -> used);
	printf("id : %d\n", i -> id);
	printf("size : %d\n", i -> size);
	printf("data : %u\n", i -> data);
	printf("directory : %d\n", i -> directory);
	printf("last_accessed : %d\n", i -> last_accessed);
	printf("last_modified : %d\n", i -> last_modified);
	printf("link_count : %d\n", i -> link_count);
  printf(RESET);
}




//FUSE Functions



  

  static int turbo_getattr(const char *path, struct stat *stbuf,struct fuse_file_info *fi)
  {
  	

  	int res = 0;

  	char *path2;
  	path2 = (char *)malloc(sizeof(path));
  	strcpy(path2, path);

    //buffer to get the attributes
  	memset(stbuf, 0, sizeof(struct stat));

  	int ino;
    //printf(GRN BOLD "getattr calling path_to_inode\n" RESET);
  	path_to_inode(path, &ino); //find inode using the path

  	

  	//printf("before isDir path - %s\n", path2);
  	int directory_flag = isDir(path2); //chek if it is a directory

  	if (directory_flag == 1) 
    {
  		//printf("getattr - Directory\n");
  		stbuf->st_mode = S_IFDIR | 0777;
  		stbuf->st_nlink = 2;
  	}

  	else if (directory_flag == 0)
    {
      inode *temp_ino = inodes + (ino * sizeof(inode));
  		stbuf->st_mode = S_IFREG | 0444;
  		stbuf->st_nlink = 1;
  		stbuf->st_size = temp_ino -> size;
  	}

  	else
    {
  		res = -ENOENT;
  	}
  	return res;
  }

  static int turbo_readdir(const char *path, void *buf, fuse_fill_dir_t filler,off_t offset, struct fuse_file_info *fi)
  {
 
  	(void) offset;
  	(void) fi;
  	

  	int ino; //inode index in the array
    //printf(GRN BOLD "readdir calling path_to_inode\n" RESET);
  	path_to_inode(path, &ino);// read the path to find the inode

  	//printf("ino returned for %s - %d\n", path, ino);

    //if inode not found
  	if (ino == -1)
    {
  		return -ENOENT;
  	}

  	else
    {
  		dirent *temp;
  		filler(buf, ".", NULL, 0);
  		filler(buf, "..", NULL, 0);
  		//printf("filled some stuff\n");

      //if directory is root
  		if(strcmp(path, "/") == 0)
      {
  			temp = root_directory;
  		}

      //get the dirent using inode found at inode number
  		else
      {
        inode *temp_ino = inodes + (ino * sizeof(inode));
  			temp = (dirent *)(datablks + ((temp_ino -> data) * block_size));
  			
  		}

      //read all files/entries in the DIR in a loop
  		while((strcmp(temp -> filename, "") != 0))
      {
  			//printf("I'm here\n");
  			filler(buf, temp -> filename, NULL, 0);
  			temp++;
  			//temp = (dirent *)temp
  		}
  	}
   
  	return 0;
  }


  static int turbo_mkdir(const char *path, mode_t mode)
  {
    //ALLOCATE AND SET new directory

    //find free inode -> set inode parameters at index
  	int ino = return_first_unused_inode(inode_map);
  	allocate_inode(path, &ino, true);

    //access the inode
    inode *temp_ino = inodes + (ino * sizeof(inodes));
  	print_inode(temp_ino);
  	//printf("inode address for %s - %u\n", path, ino);


  	// go through the path
  	char *token = strtok(path, "/");
  	dirent *temp;
  	temp = root_directory;

  	while (token != NULL)
    {
  		//printf("token = %s\n", token);
  		//printf("temp -> filename = %s\n", temp->filename);

  		while((strcmp(temp -> filename, "") != 0) && (strcmp((temp -> filename), token) != 0))
      {
  			temp++;
  		}

  		if((strcmp(temp -> filename, "") == 0))
      {
  			// Add the new directory to temp
  			//printf("HERE\n");
  			temp = (dirent *)temp;
  			//printf("temp = %u\n temp -> filename = %u\n", temp, temp -> filename);
  			//(temp -> filename) = (char *)malloc(15);
  			strcpy((temp -> filename), token);
  			temp -> file_inode = ino;

        printf("\n\ndirectory created on disk as well\n\n");
        lseek(turbo_file, 0, SEEK_SET);
        write(turbo_file, fs, FS_SIZE);
  			return 0;
  		}
  		else
      {
        temp_ino = (inodes + ((temp -> file_inode) * sizeof(inode)));
  			if(temp_ino -> directory)
        {
  				temp = (dirent *)(datablks + ((temp_ino -> data) * block_size));
  			}
  		}
  		token = strtok(NULL, "/");
  	}

    
  }

//remove a directory only if the directory is empty
  static int turbo_rmdir(const char *path)
  {

  	int i;
    //start from back and get the last directory
  	i = strlen(path);
  	while(path[i] != '/')
    {
  		i--;
  	}


  	char *subpath;
  	subpath = (char *)malloc(strlen(path)+ 1);

    //basically not including the non directory part
  	strncpy(subpath, path, i);
  	subpath[i] = '/';
  	subpath[i+1] = '\0';


  	int ino;
    dirent *temp;
    dirent *temp_data;
    inode *temp_ino;

    //if root 
    if(strcmp(subpath, "/") == 0)
    {
      temp = root_directory;
    }

    //get inode number using the original path given -> and thus the dirent
    else
    {
      printf(GRN BOLD "rmdir calling path_to_inode\n" RESET);
      path_to_inode(subpath, &ino);
      //printf("Inode for the path - %s - %d\n", path, ino);

      temp_ino = inodes + (ino * sizeof(inode)); //get inode at offset

    	temp = datablks + ((temp_ino->data) * block_size); // get dirent using the inode
    }

  	
    //do similar for the subpath = to check if the directory is empty or not

  	//printf("ino -> data = %u\n", temp);
  	//printf("root_directory = %u\n", root_directory);

  	while(((strcmp(temp -> filename, "") != 0) || (temp -> file_inode) != 0) && strcmp(path + (i+1), temp -> filename) != 0)
    {
  		temp ++;
  	}

  	if((strcmp(temp -> filename, "") != 0))
    {
      //printf("I'm here!!");
      //printf("temp -> file_inode = %d\n", temp -> file_inode);

  		if(temp -> file_inode != 0)
      {
        temp_ino = inodes + ((temp -> file_inode) * sizeof(inode));
        temp_data = ((dirent *)(datablks + ((temp_ino -> data) * block_size)));

        //directory has stuff
        if(strcmp((temp_data -> filename), "") != 0)
        {
  				
          #ifdef DEBUG
  				printf("Directory isnt empty .. so cannot rmdir!!\n");
  				#endif

  				return -1;
  			}


        //if it is empty -> free the bitmaps(inode and the databitmap)

  			strcpy(temp ->filename, "");
  			data_map[(temp_ino -> data)] = 1;
  			inode_map[(temp -> file_inode)] = 0;

  		}
  	}


    lseek(turbo_file, 0, SEEK_SET);
    write(turbo_file, fs, FS_SIZE);

  	return 0;
  }


// create new file -> for touch
  static int turbo_create(const char *path, mode_t mode,struct fuse_file_info *fi)
  {

    //find free inode and set the params for inode
  	int ino = return_first_unused_inode(inode_map);
  	allocate_inode(path, &ino, false);


    //start at the root
  	char *token = strtok(path, "/");
  	dirent *temp;
  	temp = root_directory;
  	


  	char *file = (char *)malloc(15);
  	while (token != NULL)
    {
  		
      //loop till the end of the path
  		while((strcmp(temp -> filename, "") != 0) && (strcmp(temp -> filename, token) != 0))
      {
  			temp++;
  		}

      //if it is end of path , attach the inode and set dirent
  		if((strcmp(temp -> filename, "") != 0))
      {
          inode *temp_ino = inodes + ((temp -> file_inode) * sizeof(inode));
  				if(temp_ino -> directory)
          {
  					temp = datablks + ((temp_ino -> data) * block_size);
  				}
  				else
          {
  					return -1;
  				}
  		}

  		strcpy(file, token);
  		token = strtok(NULL, "/");
  	}



  	strcpy(temp -> filename, file);
  	temp -> file_inode = ino;
    printf("\n\ncreating on disk!!\n\n");
    lseek(turbo_file, 0, SEEK_SET);
    printf("%d\n", write(turbo_file, fs, FS_SIZE));
    
  	return 0;

  }


  //just go through the path and find the inode number
  //it us basically the fd for the file
  static int turbo_open(const char *path, struct fuse_file_info *fi)
  {
  	int ino;
    printf(GRN BOLD "open calling path_to_inode\n" RESET);
  	path_to_inode(path, &ino);
  	//printf("inode returned = %u\n", ino);

  	if(ino == -1)
    {
  		return -1;
  	}
    
  	return 0;
  }


  static int turbo_read(const char *path, char *buf, size_t size, off_t offset,struct fuse_file_info *fi)
  {
  	int ino;
    printf(GRN BOLD "read calling path_to_inode\n" RESET);
  	path_to_inode(path, &ino);
  	size_t len;
  	(void) fi;

  	if(ino == -1)
  		return -ENOENT;

    inode *temp_ino = (inodes + (ino * sizeof(inode)));
  	len = temp_ino->size;

  	if (offset < len) 
    {
  		if (offset + size > len)
  			size = len - offset;
      char *temp_data = datablks + ((temp_ino -> data) * block_size);
  		memcpy(buf, temp_data + offset, size);
  	} 

    else
  		size = 0;
    
  	return size;
  }


  static int turbo_write(const char *path, const char *buf, size_t size,off_t offset, struct fuse_file_info *fi)
  {
  
  	//printf("Write called!!\n");
  	
  	int ino;
    printf(GRN BOLD "write calling path_to_inode\n" RESET);
  	path_to_inode(path, &ino);
    inode *temp_ino = inodes + (ino * sizeof(inode));
  	memcpy(((datablks + ((temp_ino -> data) * block_size)) + offset), (buf), size);
  	temp_ino -> size = (temp_ino -> size) +  size;

    printf("\n\nwriting to disk\n\n");
    lseek(turbo_file, 0, SEEK_SET);
    printf("%d\n", write(turbo_file, fs, FS_SIZE));
    
    return 0;

  }



//same as rm_dir but no need to check if empty or not
  static int turbo_rm(const char *path)
  {

    int i;
  	i = strlen(path);
  	while(path[i] != '/')
    {
  		i--;
  	}

  	char *subpath;
  	subpath = (char *)malloc(strlen(path)+ 1);
  	strncpy(subpath, path, i);
  	subpath[i] = '/';
  	subpath[i+1] = '\0';

  	int ino;
    dirent *temp;
    dirent *temp_data;
    inode *temp_ino;

    //if root
    if(strcmp(subpath, "/") == 0)
    {
      temp = root_directory;
    }

    //original path
    else
    {
      printf(GRN BOLD "rm calling path_to_inode\n" RESET);
      path_to_inode(subpath, &ino);
     // printf("Inode for the path - %s - %d\n", path, ino);

      temp_ino = inodes + (ino * sizeof(inode));

    	temp = datablks + ((temp_ino->data) * block_size);
    }

    //subpath

  	//printf("ino -> data = %u\n", temp);
  	//printf("root_directory = %u\n", root_directory);

  	while(((strcmp(temp -> filename, "") != 0) || (temp -> file_inode) != 0) && strcmp(path + (i+1), temp -> filename) != 0){
  		temp ++;
  	}


  	if((strcmp(temp -> filename, "") != 0))
    {
      //printf("temp -> file_inode = %d\n", temp -> file_inode);
  		if(temp -> file_inode != 0)
      {
        temp_ino = inodes + ((temp -> file_inode) * sizeof(inode));
        temp_data = ((dirent *)(datablks + ((temp_ino -> data) * block_size)));
  			strcpy(temp ->filename, "");
  			//data_map[(temp_ino -> data)] = 1;
  			inode_map[(temp -> file_inode)] = 0;
  		}
  	}


    printf("\n\nfile deleted from disk\n\n");
    lseek(turbo_file, 0, SEEK_SET);
    write(turbo_file, fs, FS_SIZE);
    printf("____________________________________________________________\n");
  	return 0;

}
