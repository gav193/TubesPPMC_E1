#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX 256

int main() {
    char filename[MAX];
    // input filename from user
    printf("Enter file name : ");
    scanf("%s", filename);

    FILE *stream = fopen(filename, "r");
    if (stream == NULL) { // file not found within directory (return error message and end program)
        printf("File not found!\n");
        return 1;
    }

    int row = 0;
    int col = MAX;
    char line[MAX]; // assumption: no lines have varying lengths
    while (fgets(line, MAX, stream)) {
        printf("%s \n", line);
        row++; // save number of rows (lines)
        int cur = strlen(line);
        if (cur < col) {
            col = cur; // save the number of cols with no newline char
        }
    }

    rewind(stream);

    char map[row][col];
    int track = 0;

    while (fgets(line, MAX, stream)) {
        line[strcspn(line,"\n")] = '\0';  
        strncpy(map[track], line, col-1); // remove newline char
        map[track][col-1] = '\0';
        track++;
    }
    // parse into matrix of char complete
    fclose(stream);

    return 0;
}
