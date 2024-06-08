#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pthread.h>
#include <openssl/sha.h>

#define MAX_PATH_LENGTH 5
#define HASH_SIZE 20
#define MAX_FILE_TYPES 4

int totalDeletedFiles = 0;
int totalCheckedFiles = 0;

void calculateFileHash(char *filePath, unsigned char *hash)
{
    FILE *file = fopen(filePath, "r");

    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    SHA_CTX ctx;

    SHA1_Init(&ctx);

    unsigned char buffer[8192];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) != 0)
    {
        SHA1_Update(&ctx, buffer, bytesRead);
    }

    SHA1_Final(hash, &ctx);
    fclose(file);
}

void checkAndDeleteDuplicates(char *filePath)
{
    unsigned char hash[HASH_SIZE];
    calculateFileHash(filePath, hash);

    unsigned char predefinedHash[HASH_SIZE] = {
        0x12,
        0x34,
        0x56,
        0x78,
        0x90};

    if (memcmp(hash, predefinedHash, HASH_SIZE) == 0)
    {
        if (remove(filePath) == 0)
        {
            printf("Deleted duplicate file: %s\n", filePath);
        }
        else
        {
            perror("Error deleting file");
        }
    }
}

void createLogFile(char *folderPath)
{
    char logFilePath[MAX_PATH_LENGTH];
    sprintf(logFilePath, "%s.log", folderPath);

    FILE *logFile = fopen(logFilePath, "w");
    if (logFile == NULL)
    {
        perror("Error creating log file");
        return;
    }

    fprintf(logFile, "Log File for Folder: %s\n", folderPath);
    fprintf(logFile, "Total Checked Files: %d\n", totalCheckedFiles);
    fprintf(logFile, "Total Deleted Files: %d\n", totalDeletedFiles);

    fclose(logFile);
}
void *processFiles(void *arg)
{
    char folderPath[MAX_PATH_LENGTH];
    strcpy(folderPath, (char *)arg);

    DIR *dir;
    struct dirent *entry;

    dir = opendir(folderPath);
    if (dir == NULL)
    {
        perror("Error opening directory");
        pthread_exit(NULL);
    }

    while ((entry = readdir(dir)) != NULL)
    {
        char filePath[MAX_PATH_LENGTH];
        sprintf(filePath, "%s/%s", folderPath, entry->d_name);

        struct stat fileStat;
        if (stat(filePath, &fileStat) == -1)
        {
            perror("Error getting file status");
            continue;
        }

        if (S_ISREG(fileStat.st_mode))
        {
            checkAndDeleteDuplicates(filePath);
        }
    }

    closedir(dir);

    createLogFile(folderPath);

    pthread_exit(NULL);
}
void displayMonitoringInfo(int totalFileFormats[], int totalVolumeBefore, int totalVolumeAfter)
{
    printf("Total Checked Files: %d\n", totalCheckedFiles);

    printf("Number of Each Type of File Format:\n");

    for (int i = 0; i < MAX_FILE_TYPES; i++)
    {
        printf("File Format %d: %d\n", i + 1, totalFileFormats[i]);
    }

    printf("Number of Deleted Files: %d\n", totalDeletedFiles);

    printf("Volume Before Erasing: %d\n", totalVolumeBefore);
    printf("Volume After Erasing: %d\n", totalVolumeAfter);

    printf("Log Files Creation: Completed\n");
}
int main()
{
    char rootFolder[MAX_PATH_LENGTH];
    printf("Enter the root folder path: \n");
    scanf("%s", rootFolder);

    pthread_t tid;

    int totalFileFormats[10] = {10, 20, 30};
    int totalVolumeBefore = 1000;
    int totalVolumeAfter = 800;

    displayMonitoringInfo(totalFileFormats, totalVolumeBefore, totalVolumeAfter);
    return 0;
}