#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX 256

int main() {
    char filename[MAX];
    // input filename from user
    printf("Enter file name: ");
    scanf("%s", filename);

    FILE *stream = fopen(filename, "r");
    if (stream == NULL) {
        printf("File not found!\n");
        return 1;
    }

    int row = 0;
    char line[MAX];
    char map[MAX][MAX];

    fgets(line, sizeof(line), stream);
    int col = strlen(line) -1; // initialize column value as line length (-1 because of newline char)
    rewind(stream);

    while (fgets(line, sizeof(line), stream)) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline character
        strcpy(map[row], line);
        row++;
        if (col != strlen(line)) {
            printf("Found uneven row of line at %d\n, row);
            exit(1);
        }
    }

    fclose(stream);

    return 0;
}
