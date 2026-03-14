#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "patchFunctions.h"
#include "fileFunctions.h"

int copyFileContent (aimFile_t* aimFile) {
    assert(aimFile);

    int fileDescriptor = open(aimFile->name, O_RDONLY, 0);
    if (fileDescriptor == -1) {

        fprintf(stderr, "Error of opening file \"%s\"", aimFile->name);
        perror("");
        return 1;
    }

    aimFile->size = getSizeOfFile(fileDescriptor);
    if (aimFile->size == 0) {
        close(fileDescriptor);
        return 1;
    }

    char* fileCopyBuffer = (char*)calloc(aimFile->size, sizeof(char));

    read(fileDescriptor, fileCopyBuffer, aimFile->size);

    if(close(fileDescriptor) != 0) {
        fprintf(stderr, "Error of closing file \"%s\"", aimFile->name);
        perror("");
        return 1;
    }

    aimFile->bufferCopy = fileCopyBuffer;
    return 0;
}

unsigned int getSizeOfFile (int fileDescriptor) {
    struct stat fileInfo = {};

    if (fstat(fileDescriptor, &fileInfo) == 0)
        return fileInfo.st_size;

    perror("Error of getting the size of the file");
    return 0;
}
