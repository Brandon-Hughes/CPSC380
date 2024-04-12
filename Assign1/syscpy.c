/// @brief Takes in an input file and copies it to an output file using System Calls
/// @file syscpy.c
/// @author Brandon Hughes
/// @course CPSC-380-01 Operating Systems
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv) {
    char* readfile = argv[1];
    char* writefile = argv[2];
    char buf[20];
    size_t nbytes = 20;
    ssize_t reading_input;
    ssize_t writing_output;

    //opening the input file
    int rf = syscall(SYS_open, readfile, O_RDONLY);
    if (errno == ENOENT){
        printf("An error occured while trying to open the file.");
        return 0;
    }

    //creating the output file
    int wf = syscall(SYS_open, writefile, O_WRONLY | O_EXCL | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (errno == EEXIST){
        printf("An error occured while trying to create the file.");
        return 0;
    }
    
    //reading from input file
    reading_input = syscall(SYS_read, rf, buf, 20);
    if(reading_input == -1){
        printf("An error occured while trying to read the file.");
        return 0;
    }

    while(reading_input != 0){

        //writing to output file
        if (strlen(buf) < 20){
            nbytes = strlen(buf);
        }
        writing_output = syscall(SYS_write, buf, nbytes);
        if(writing_output == -1){
            printf("An error occured while trying to write to the file.");
            return 0;
        }

        //clearing buffer
        memset(buf, '\0', 20);

        //reading the input file for more lines
        reading_input = syscall(SYS_read, buf, 20);
        if(reading_input == -1){
            printf("An error occured while trying to read the file.");
            return 0;
        }
    }

    //closing files
    syscall(SYS_close, rf);
    syscall(SYS_close, wf);
    printf("File has successfully been copied");

    return 0;
}