#include<stdio.h>
#include<dirent.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include <iomanip>

using namespace std;

struct dirent* dir_data;
struct stat file_data;

bool decision(int flag, int inode, string file, string filename, float size_mb, int min,  int max){
	
    if( size_mb >= (float)min && size_mb <= (float)max){
        if(inode >= 0 && flag == inode)
            return true;
        else if(inode < 0 && !strcmp(file.c_str(), filename.c_str()))
            return true;
        else if(inode < 0 && filename == "original")
            return true;
    }
    return false;
}

int find(string pathname, string filename, string dirname ,int inode, int min, int max){
	
	DIR* to_opendir;
	string routine, directory, file;
	int recursive_inode;
	float size_mb;
	
	stat(pathname.c_str(), &file_data);
	size_mb = (float)file_data.st_size/(float)1048576;	
	
    if(decision(file_data.st_ino, inode, dirname, filename,size_mb, min, max))
        cout << pathname << " " << file_data.st_ino << " " << fixed << setprecision(1) << size_mb << " MB" << endl;

	to_opendir = opendir(pathname.c_str());

	for( dir_data = readdir(to_opendir); dir_data != NULL; dir_data = readdir(to_opendir)){
		
		if( !strcmp(dir_data->d_name, ".") || !strcmp(dir_data->d_name, ".."))
				continue;
		
			if( dir_data->d_type == DT_DIR){						//a directory
				directory = dir_data-> d_name;
				recursive_inode = find(pathname + "/" + directory , filename, directory , inode, min, max);
				
				if(recursive_inode == inode)
					return inode;
			}
	  	  	else if( dir_data->d_type == DT_REG){						//a file
				file = dir_data-> d_name;
				routine = pathname + '/' + file;
				stat(routine.c_str(), &file_data);
					
				size_mb = (float)file_data.st_size/(float)1048576;
			
			if(decision(file_data.st_ino, inode, file, filename, size_mb, min, max))
				cout<< routine << " " << file_data.st_ino << " " << fixed << setprecision(1) << size_mb << " MB" << endl;
        }
    }
    return 0;
}

int main(int argc, char **argv){

	string filename = "original";
	int inode = -1;
	int min = 0, max = 9999999;

	if(argc == 1)
		exit(1);
	string pathname = argv[1];
	
	for(int i = 2;i < 8 ; i++){
		if(argv[i] == NULL) break;
		if(!strcmp(argv[i], "-name"))
			filename = argv[i+1];
		else if(!strcmp(argv[i],"-inode"))
			inode = atoi(argv[i+1]);
		else if(!strcmp(argv[i],"-size_min"))
			min = atoi(argv[i+1]);
		else if(!strcmp(argv[i],"-size_max"))
			max = atoi(argv[i+1]);
		else;
	}
	find(pathname ,filename ,pathname ,inode , min ,max);
	return 1;
}


           



