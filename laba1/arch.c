#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void PrintZip(char *dir, FILE *outputFile, char *archName);
void CreateZip(char *dir, FILE *outputFile, char *archName);
void ReadZip(char *dir);
void CheckFolder(char *dir);
void ReturnZipFolder(char *dir);

int main(int argc, char *argv[]) {
    char *topdir = ".";
    int flagArch;
    if (argc > 2 && atoi(argv[2]) == 1) {
        topdir = argv[1];
        flagArch = atoi(argv[2]);
    } else if (argc == 2 && atoi(argv[1]) == 2) {
        flagArch = atoi(argv[1]);
    } else {
        printf("Error");
        return 0;
    }

    if (flagArch == 1) {
        FILE *outputFile = fopen("output.bin", "wb");
        char *str = malloc(strlen(topdir));
        strcpy(str, topdir);
        ReturnZipFolder(str);
        PrintZip(topdir, outputFile, str);
        fclose(outputFile);
        free(str);
    } else if (flagArch == 2) {
        ReadZip("output.bin");
    } else {
        printf("Error");
    }
    return 0;
}

void PrintZip(char *dir, FILE *outputFile, char *archName) {
    DIR *dp;
    struct dirent *entry;
    struct stat statBuf;
    if ((dp = opendir(dir)) == NULL) {
        printf("Error!\n");
        return;
    }

    chdir(dir);
    while ((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name, &statBuf);
        if (S_ISDIR(statBuf.st_mode)) {
            if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0) {
                continue;
            }
            PrintZip(entry->d_name, outputFile, archName);
        } else {
            CreateZip(entry->d_name, outputFile, archName);
        }
    }

    chdir("..");
    closedir(dp);
}

void CreateZip(char *dir, FILE *outputFile, char *archName) {
    FILE *inputFile = fopen(dir, "rb");

    char buffer[1024], orig_dir[1024];
    int bytesRead = fread(buffer, 1, sizeof(buffer), inputFile);

    for (size_t i = 0; i < strlen(dir); i++) {
        if (dir[i] == 32) {
            dir[i] = '_';
        }
    }
    strcpy(orig_dir, strstr(getcwd(orig_dir, sizeof(orig_dir)), archName));
    strcat(orig_dir, "/");
    printf("%s\n", orig_dir);
    system("clear");
    fwrite(orig_dir, 1, strlen(orig_dir), outputFile);
    fwrite(dir, 1, strlen(dir), outputFile);
    fprintf(outputFile, " %d ", bytesRead);
    fwrite(buffer, 1, bytesRead, outputFile);
    fclose(inputFile);
}

void ReadZip(char *dir) {
    system("clear");
    FILE *inputFile = fopen(dir, "rb"), *outputFile;
    char fileName[256], fileSize[100];
    int c, flagNameRead = 1, flagSizeRead = 0, i_name = 0, i_size = 0;
    while ((c = fgetc(inputFile)) != EOF) {
        if (flagNameRead && c != 32) {
            fileName[i_name] = c;
            i_name++;
        }
        if (flagSizeRead && c != 32) {
            fileSize[i_size] = c;
            i_size++;
        }
        if (c == 32) {
            if (flagNameRead) {
                flagNameRead = 0;
                flagSizeRead = 1;
                i_name = 0;
            } else {
                CheckFolder(fileName);
                flagNameRead = 1;
                flagSizeRead = 0;
                outputFile = fopen(fileName, "wb");

                memset(fileName, 0, sizeof(fileName));
                int size = atoi(fileSize);
                while (size--) {
                    c = fgetc(inputFile);
                    fputc(c, outputFile);
                }
                memset(fileSize, 0, sizeof(fileSize));
                fclose(outputFile);
                i_size = 0;
            }
        }
    }
}

void CheckFolder(char *dir) {
    char idir[1024];
    int index = 0, c = 0;
    memset(idir, 0, sizeof(idir));
    for (size_t i = 0; i < strlen(dir); i++) {
        if (dir[i] == '/') {
            c++;
        }
    }
    for (size_t i = 0; i < strlen(dir) && c > 0; i++) {
        if (dir[i] == '/') {
            DIR *folder = opendir(idir);
            if (!folder) {
                mkdir(idir, 0777);
            } else {
                closedir(folder);
            }
        }
        idir[index] = dir[i];
        index++;
    }
}

void ReturnZipFolder(char *dir) {
    char tmp_dir[strlen(dir)];
    int index = 0;
    for (size_t i = strlen(dir) - 1; i > 0; i--) {
        if (dir[i] == '/') {
            for (int k = 0, m = index - 1; k < m; k++, m--) {
                char ch = tmp_dir[k];
                tmp_dir[k] = tmp_dir[m];
                tmp_dir[m] = ch;
            }
            strcpy(dir, tmp_dir);
        } else {
            tmp_dir[index] = dir[i];
            index++;
        }
    }
}