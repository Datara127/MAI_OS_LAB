#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void PrintZip(char *dir, int depth);
void CreateZip(char *dir, FILE *outputFile);
void ReadZip(char *dir);

int main(int argc, char *argv[]) {
    char *topdir = ".";
    if (argc > 1) {
        topdir = argv[1];
    }

    PrintZip(topdir, 0);
    ReadZip("output.bin");
    return 0;
}

void PrintZip(char *dir, int depth) {
    DIR *dp;
    struct dirent *entry;
    struct stat statBuf;
    if ((dp = opendir(dir)) == NULL) {
        printf("Error!\n");
        return;
    }

    chdir(dir);
    FILE *outputFile = fopen("../output.bin", "wb");
    while ((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name, &statBuf);
        if (S_ISDIR(statBuf.st_mode)) {
            if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0) {
                continue;
            }
            printf("%*s%s/\n", depth, "", entry->d_name);
            PrintZip(entry->d_name, depth + 4);
        } else 