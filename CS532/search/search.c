/*
Name: Maseera Khan
BlazerId: mkhan52
Project: HW2
To compile: make
To run: ./search [-S] [-s size] [-f pattern depth] [-t f|d] [directory]
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<sys/stat.h>
#include<unistd.h>
#include<time.h>

#define PATHSIZE 1024

void traverseDir(char *dirname, int depth);
void printTabs(int depth);
void printNormal(char *name, char *path, struct stat *statbuf, int depth);
void printWithInfo(char *name, char *path, struct stat *statbuf, int depth);
int shouldPrint(char *name, struct stat *statbuf, int depth);

void (*printFunction)(char *, char *,struct stat *, int);

int useSizeFilter = 0;
long maxSize = 0;
int usePatternFilter = 0;
int maxDepth = 0;
int useTypeFilter = 0;
char typeFilter;
char pattern[256];

int main(int argc, char *argv[]){

    char *startDir;
    int opt;

    printFunction = printNormal;

    //process command-line options using getopt
    while ((opt = getopt(argc, argv, "Ss:f:t:")) != -1)
    {
        switch(opt)
        {
            case 'S':
                printFunction = printWithInfo;
                break;

            case 's':
                useSizeFilter =1;
                maxSize = atol(optarg);
                break;
            
            case 'f':
                usePatternFilter = 1;
                strcpy(pattern, optarg);

                if (optind < argc)
                {
                    maxDepth = atoi(argv[optind]);
                    optind++;

                }
                else
                {
                    printf("Error: -f requires a pattern and depth.\n");
                    return 1;

                }

                break;

            case 't':
                useTypeFilter =1;
                typeFilter = optarg[0];

                if (typeFilter != 'f' && typeFilter != 'd')
                {
                    printf("Error: -t option must be f or d.\n");
                    return 1;
                }

                break;

            default:
                printf("Usage: ./search [-S] [-s size] [-f pattern depth] [-t f|d] [directory]\n");
                return 1;

        }
    }

    if (optind < argc)
    {
        startDir = argv[optind];
    }

    else
    {
        startDir = ".";
    }

    printf("%s\n", startDir);

    traverseDir(startDir, 1);

    return 0;
}

void printTabs(int depth){
    int i;

    //print one tab for each level of depth in the directory tree
    for(i=0;i<depth; i++)
    {
        printf("\t");
    }
}

//decides if an item passes the filter
int shouldPrint(char *name, struct stat *statbuf, int depth)
{
    // -t
    if (useTypeFilter == 1)
    {
        if (typeFilter == 'f' && !S_ISREG(statbuf->st_mode))
        return 0;

        if (typeFilter == 'd' && !S_ISDIR(statbuf->st_mode))
        return 0;
    }

    // -s
    if (useSizeFilter == 1 && S_ISREG(statbuf->st_mode))
    {
        if(statbuf->st_size > maxSize)
        {
            return 0;
        }
    }

    // -f
    if (usePatternFilter ==1)
    {
        if (strstr(name, pattern)== NULL)
        {
        return 0;
        }
    

        if (depth > maxDepth)
        {
            return 0;
        }

    }
return 1;
}

//prints normal output
void printNormal(char *name, char *path, struct stat *statbuf, int depth)
{
    // print the file or directories name using tree indentation
    printTabs(depth);
    printf("%s\n", name);
}

void traverseDir(char *dirname, int depth)
{
    DIR *parentDir;
    struct dirent *entry;
    struct stat statbuf;
    char path[PATHSIZE];
    char linkTarget[PATHSIZE];
    int linklength;

    // open the directory passed to the function
    parentDir = opendir(dirname);

    if (parentDir == NULL)
    {
        printf("Error opening the directory '%s'\n", dirname);
        return;
    }

    while ((entry = readdir(parentDir)) != NULL)
    {
        //skip current directory "." and parent directory ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        continue;
        
        //build the full path 
        snprintf(path, PATHSIZE, "%s/%s", dirname, entry->d_name);
        

        //get file information such as type, size, permissions, and access time
        if(lstat(path, &statbuf)< 0)
        {
            printf("lstat error for %s\n", path);
            continue;
        }

        if (shouldPrint(entry->d_name, &statbuf, depth))
        {
            //Print symbolic link with the file it points to
            if (S_ISLNK(statbuf.st_mode))
            {
                linklength = readlink(path, linkTarget, PATHSIZE -1);
                
                if(linklength >= 0)
                {
                    linkTarget[linklength] = '\0';

                    printTabs(depth);
                    printf("%s (%s)\n", entry->d_name, linkTarget);

                }
            }
            else
            {
                printFunction(entry->d_name, path, &statbuf, depth);
            }
        }
        // Recursively enter subdirectories
        if(S_ISDIR(statbuf.st_mode))
        {
            traverseDir(path, depth +1);
        }
    }

    closedir(parentDir);
}

// prints detailed -S output
void printWithInfo(char *name, char *path, struct stat *statbuf, int depth){

    long size;
    char accessTime[100];

    printTabs(depth);
    
    if (S_ISDIR(statbuf->st_mode))
    {
        size =0;
    }
    else{
        size = statbuf->st_size;
    }

    //convert last access time into readable date/time format
    strftime(accessTime, 100, "%a %b %d %H:%M:%S %Y", 
        localtime(&statbuf->st_atime));

    //print detailed information for the -S option
    printf("%s (size: %ld, permissions: %o, last access: %s)\n",
        name, size, statbuf->st_mode & 0777, accessTime);

}
    