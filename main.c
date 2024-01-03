#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void process_directory( char *directory,
                        int *txt_count,
                        int *pdf_count,
                        int *jpg_count,
                        int *png_count,
                        long long *total_size,
                        long long *min_size,
                        char *min_file,
                        long long *max_size,
                        char *max_file
                        ) 
    {

    DIR *dir;
    struct dirent *ent;
    struct stat buf;
    char path[2048];

    if ((dir = opendir(directory)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            sprintf(path, "%s/%s", directory, ent->d_name);
            if (stat(path, &buf) == 0) {
                if (S_ISDIR(buf.st_mode)) {
                    // pid_t pid = fork();
                    // if(pid == 0) {
                        // printf("the child is executed");
                        if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
                            process_directory(path, txt_count, pdf_count, jpg_count, png_count, total_size, min_size, min_file, max_size, max_file);
                        }
                    // }
                } else {
                    *total_size += buf.st_size;
                    if (buf.st_size < *min_size) {
                        *min_size = buf.st_size;
                        strcpy(min_file, path);
                    }
                    else if (buf.st_size > *max_size) {
                        *max_size = buf.st_size;
                        strcpy(max_file, path);
                    }
                    char *ext = strrchr(ent->d_name,'.');
                    if (ext != NULL) {
                        if (strcmp(ext, ".txt") == 0) (*txt_count)++;
                        if (strcmp(ext, ".pdf") == 0) (*pdf_count)++;
                        if (strcmp(ext, ".jpg") == 0) (*jpg_count)++;
                        if (strcmp(ext, ".png") == 0) (*png_count)++;
                    }
                }
            }
        }
        closedir(dir);
    } else {
        printf("no such file\n");
    }
}

int main() {
    long long total_size = 0, min_size = 1000, max_size = 0;
    int txt_count = 0, pdf_count = 0, jpg_count = 0, png_count = 0;
    char min_file[2048], max_file[2048];

    char directory[1024];
    printf("Please enter the directory path: ");
    scanf("%s", directory);

    process_directory(directory, &txt_count, &pdf_count, &jpg_count, &png_count, &total_size, &min_size, min_file, &max_size, max_file);

    printf("Total number of files: %d\n", txt_count + pdf_count + jpg_count + png_count);
    printf("Number of each file type:\n");
    printf("- .txt: %d\n", txt_count);
    printf("- .pdf: %d\n", pdf_count);
    printf("- .jpg: %d\n", jpg_count);
    printf("- .png: %d\n", png_count);
    printf("File with the largest size: %s, Size: %.2f KB\n", max_file, max_size / 1024.0);
    printf("File with the smallest size: %s, Size: %.2f KB\n", min_file, min_size / 1024.0);
    printf("Size of the root folder: %.2f KB\n", total_size / 1024.0);

    return 0;
}