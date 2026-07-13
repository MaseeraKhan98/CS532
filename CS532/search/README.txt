HW 2 - Search Program

Name: Maseera Khan  
Blazer ID: mkhan52
Project: Homework 2


#Description
This implements a simplified search command in C. 
The program recursively traverses a directory hierarchy and print files and directories. 
If no file is given, the program starts from the current directory.
The program uses directory system calls such as opendir(), readdir(), closedir(),lstat() and readlink().
It also uses a function pointer to choose between normal printing and detailed printing for -S option.

#Options Supported:

-S
Prints detailed information for each file or directory. The information includes size, permissions, and last access time.
For directories, the size is printed as 0 as required by the assignment.

-s <size>
Prints regular files whose size is less than or equal to the given size in bytes.

-f <pattern> <depth>
Prints entries whose name contains the given pattern and whose depth is less than or equal to the given depth.
The starting directory is considered depth 0.

-t f
Prints regular files only.

-t d
Prints directories only.

Symbolic Links:
Symbolic links are printed in the required format
Linkname (target)

#Compile:
make

#Run:
./search [options][directory]

#Examples:
./search
./search projects
./search -S projects
./search -s 1024 projects
./search -f docx 2 projects
./search -S -s 1024 -f docx 2 projects
./search -t f projects
./search -t d projects

#Testing:
I tested the program using the sample projects folder provided with the assignment. 
I extracted the file using:
tar xvf projects.tar

Then I tested the following commands:

./search projects
./search -S projects
./search -s 1024 projects
./search -f docx 2 projects
./search -S -s 1024 -f docx 2 projects
./search -t f projects
./search -t d projects


The program correctly printed the directory hierarchy for the projects folder.
The order of the files and directories may be different from the sample output because readdir() does not guarantee alphabetical order.

I also tested the program with my own testfolder containing regular files, subdirectories, and a symbolic link.
I created the symbolic link using:

ln -s file1 testfolder/link1

Then I ran:
./search testfolder

The program correctly displayed the symbolic link as:
link1(file1)

#Additional Testing:
I tested combinations of options to make sure the order of options does not matter. Example include:
./search -S -s 1024 projects
./search -S -f docx 2 projects
./search -s 1024 -f docx 2 projects
./search -f docx 2  -s 1024 projects
./search -S -s 1024 -f docx 2 projects
./search -S -f docx 2 -s 1024 projects

I also created jpg-named files to test the pattern example from the assignment:

echo "test jpg file" > projects/photo.jpg
echo "test jpg file" > projects/project1/image.jpg
echo "test jpg file" > projects/project2/picture.jpg


#Clean:
make clean

