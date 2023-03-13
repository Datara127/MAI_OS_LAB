#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    DIR *dir = opendir("asd/folde");
    if (dir) {
        printf("papa");
        closedir(dir);
    } else {
        printf("ne papa");
    }

    return 0;
}