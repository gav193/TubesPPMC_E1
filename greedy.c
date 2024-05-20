/** EL2208 Praktikum Pemecahan Masalah dengan C 2023/2024
 *   Modul               : 9 - Tugas Besar
 *   Hari dan Tanggal    : Senin, 20 Mei 2024
 *   Nama (NIM)          : Farhan Revandi Suhirman (13222096)
 *   Nama File           : greedy.c
 *   Deskripsi           : mencari jalur dari input file txt dengan metode greedy
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//#include <unistd.h> // for sleep function
#include <time.h>

#define MAX 256

typedef struct {
    int x;
    int y;
} Cell;

typedef struct {
    int x;
    int y;
} Pos;

int isValid(int x, int y, int rows, int cols) {
    return (x >= 0 && x < rows && y >= 0 && y < cols);
}

void solveMaze(char maze[MAX][MAX], int rows, int cols, Pos start, Pos end) {
    Cell path[MAX * MAX];
    int pathLength = 0;

    maze[start.x][start.y] = '*';

    Cell current;
    current.x = start.x;
    current.y = start.y;

    while (current.x != end.x || current.y != end.y) {
        path[pathLength++] = current;

        int dx = end.x - current.x;
        int dy = end.y - current.y;

        // Check if the current position is next to the end point
        if ((abs(dx) == 1 && dy == 0) || (abs(dy) == 1 && dx == 0)) {
            current.x = end.x;
            current.y = end.y;
        } else {
            if (abs(dx) > abs(dy)) {
                if (isValid(current.x + (dx > 0 ? 1 : -1), current.y, rows, cols) && maze[current.x + (dx > 0 ? 1 : -1)][current.y] == '.') {
                    current.x += (dx > 0 ? 1 : -1);
                }
                else if (isValid(current.x, current.y + (dy > 0 ? 1 : -1), rows, cols) && maze[current.x][current.y + (dy > 0 ? 1 : -1)] == '.') {
                    current.y += (dy > 0 ? 1 : -1);
                }
                else {
                    pathLength--;
                    current = path[pathLength - 1];
                }
            } else {
                if (isValid(current.x, current.y + (dy > 0 ? 1 : -1), rows, cols) && maze[current.x][current.y + (dy > 0 ? 1 : -1)] == '.') {
                    current.y += (dy > 0 ? 1 : -1);
                }
                else if (isValid(current.x + (dx > 0 ? 1 : -1), current.y, rows, cols) && maze[current.x + (dx > 0 ? 1 : -1)][current.y] == '.') {
                    current.x += (dx > 0 ? 1 : -1);
                }
                else {
                    pathLength--;
                    current = path[pathLength - 1];
                }
            }
        }

        maze[current.x][current.y] = '*';

        // system("clear");
        // for (int i = 0; i < rows; i++) {
        //     for (int j = 0; j < cols; j++) {
        //         printf("%c ", maze[i][j]);
        //     }
        //     printf("\n");
        // }
        // printf("\n");
        //sleep(1);
    }

    path[pathLength++] = current;

    printf("Solution Path:\n");
    for (int i = 0; i < pathLength; i++) {
        printf("(%d, %d) ", path[i].x, path[i].y);
    }
    printf("\n");
}

int main() {
    char filename[MAX];
    clock_t startclk, endclk;
    double cpu_time_used;
    // input filename from user
    printf("Enter file name: ");
    scanf("%s", filename);

    FILE *stream = fopen(filename, "r");
    if (stream == NULL) {
        printf("File not found!\n");
        return 1;
    }

    int rows = 0;
    char line[MAX];
    char maze[MAX][MAX];

    fgets(line, sizeof(line), stream);
    int cols = strlen(line) -1; // initialize column value as line length (-1 because of newline char)
    rewind(stream);

    while (fgets(line, sizeof(line), stream)) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline character
        strcpy(maze[row], line);
        rows++;
        if (cols != strlen(line)) {
            printf("Found uneven row of line at %d\n, row);
            exit(1);
        }
    }

    fclose(stream);

    Pos start = { -1, -1 };
    Pos end = { -1, -1 };
    Pos curr;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (maze[i][j] == 'S') {
                start.x = i;
                start.y = j;
            }
            if (maze[i][j] == 'E') {
                end.x = i;
                end.y = j;
            }
        }
    }

    curr.x = start.x;
    curr.y = start.y;
    startclk = clock();
    solveMaze(maze, rows, cols, start, end);
    endclk = clock();
    cpu_time_used = ((double) (endclk - startclk)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", cpu_time_used);

    return 0;
}
