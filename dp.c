/** EL2208 Praktikum Pemecahan Masalah dengan C 2022/2023
 *   Modul               : 9 - Tugas Besar
 *   Hari dan Tanggal    : Senin, 20 Mei 2024
 *   Nama (NIM)          : Pierre Gavin Tan (13222097)
 *   Nama File           : dp.c
 *   Deskripsi           : mencari jalur pada file txt dengan metode dynamic progrmaming
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX 256
#define INF 999

char maze[MAX][MAX];

typedef struct {
    int length;
    int prevX, prevY;
} Cell;

Cell shortestPathLengths[MAX][MAX];

bool isValid(int x, int y, int rows, int cols) {
    return (x >= 0 && x < rows && y >= 0 && y < cols);
}

void findShortestPaths(int startX, int startY, int endX, int endY, int rows, int cols) {
    // initialize shortest path lengths table (memoization table)
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            shortestPathLengths[i][j].length = INF;
            shortestPathLengths[i][j].prevX = -1;
            shortestPathLengths[i][j].prevY = -1;
        }
    }
    shortestPathLengths[startX][startY].length = 0; // set shortest path length to start cell as 0
    bool updated = true;

    // dynamic programming to find shortest path lengths
    while (updated) {
        updated = false; // reset the flag for each iteration 
        for (int x = 0; x < rows; x++) {
            for (int y = 0; y < cols; y++) {
                if (maze[x][y] != '#') { // skip obstacles
                    int minNeighbor = shortestPathLengths[x][y].length;
                    int prevX = -1, prevY = -1;
                    // Consider all four neighbors
                    if (isValid(x - 1, y, rows, cols) && shortestPathLengths[x - 1][y].length + 1 < minNeighbor) { // Up
                        minNeighbor = shortestPathLengths[x - 1][y].length + 1;
                        prevX = x - 1;
                        prevY = y;
                    }
                    if (isValid(x + 1, y, rows, cols) && shortestPathLengths[x + 1][y].length + 1 < minNeighbor) { // Down
                        minNeighbor = shortestPathLengths[x + 1][y].length + 1;
                        prevX = x + 1;
                        prevY = y;
                    }
                    if (isValid(x, y - 1, rows, cols) && shortestPathLengths[x][y - 1].length + 1 < minNeighbor) { // Left
                        minNeighbor = shortestPathLengths[x][y - 1].length + 1;
                        prevX = x;
                        prevY = y - 1;
                    }
                    if (isValid(x, y + 1, rows, cols) && shortestPathLengths[x][y + 1].length + 1 < minNeighbor) { // Right
                        minNeighbor = shortestPathLengths[x][y + 1].length + 1;
                        prevX = x;
                        prevY = y + 1;
                    }
                    // update shortest path length if a shorter path is found
                    if (minNeighbor < shortestPathLengths[x][y].length) {
                        shortestPathLengths[x][y].length = minNeighbor;
                        shortestPathLengths[x][y].prevX = prevX;
                        shortestPathLengths[x][y].prevY = prevY;
                        updated = true; // set flag to indicate an update was made
                        if (x == endX && y == endY) { // found end point... ending traversal
                            return;
                        }
                    }
                }
            }
        }
    }
}

void printShortestPath(int endX, int endY, int startX, int startY, int row, int col) {
    if (shortestPathLengths[endX][endY].length == INF) {
        printf("No path found from 'S' to 'E'.\n");
        return;
    }
    printf("Shortest path length from 'S' to 'E': %d\n", shortestPathLengths[endX][endY].length);

    int path[MAX][2]; // to store the path coordinates (x,y)
    int length = 0;
    int x = endX, y = endY;
    while (x != -1 && y != -1) {
        path[length][0] = x;
        path[length][1] = y;
        int prevX = shortestPathLengths[x][y].prevX;
        int prevY = shortestPathLengths[x][y].prevY;
        x = prevX;
        y = prevY;
        length++;
    }

    printf("Path : \n(%d, %d)", startY, startX);
    for (int i = length - 2; i >= 0; i--) {
        printf(" -> (%d, %d)", path[i][1], path[i][0]);
    }
    printf("\n");

    printf("Shortest path route : \n");
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (i == startX && j == startY) {
                printf("S\t");
            } else if (i == endX && j == endY) {
                printf("E\t");
            } else {
                bool isPath = false;
                for (int k = 0; k < length; k++) {
                    if (i == path[k][0] && j == path[k][1]) {
                        isPath = true;
                        break;
                    }
                }
                if (isPath) {
                    printf("*\t");
                } else {
                    printf("%c\t", maze[i][j]);
                }
            }
        }
        printf("\n");
    }

    printf("\n");
}

int main() {
    // input filename from user
    char filename[MAX];
    clock_t startclk, endclk;
    double cpu_time_used;
    printf("Enter file name: ");
    scanf("%s", filename);
    FILE *stream = fopen(filename, "r");
    if (stream == NULL) {
        printf("File not found!\n");
        return 1;
    }

    int row = 0;
    char line[MAX];

    fgets(line, sizeof(line), stream);
    int col = strlen(line) - 1; // initialize column value and delete newline char
    rewind(stream);

    while (fgets(line, sizeof(line), stream)) {
        line[strcspn(line, "\n")] = '\0'; // remove newline character
        strcpy(maze[row], line);
        row++;
        if (col != strlen(line)) {
            printf("Found uneven row of line at %d\n", row);
            exit(1);
        }
    }
    fclose(stream);

    printf("Maze contents:\n");
    for (int i = 0; i < row; i++) {
        printf("%s\n", maze[i]);
    }

    // find the 'S' and 'E' position
    int startX = -1, startY = -1;
    int endX = -1, endY = -1;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (maze[i][j] == 'S') {
                startX = i;
                startY = j;
            }
            if (maze[i][j] == 'E') {
                endX = i;
                endY = j;
            }
        }
    }

    if (endX == -1 || endY == -1) {
        printf("End point 'E' not found.\n");
        return 1;
    }
    if (startX == -1 || startY == -1) {
        printf("Start point 'S' not found.\n");
        return 1;
    }

    printf("Start position: (%d, %d)\n", startY, startX);
    startclk = clock();
    findShortestPaths(startX, startY, endX, endY, row, col);
    printShortestPath(endX, endY, startX, startY, row, col);
    endclk = clock();
    cpu_time_used = ((double) (endclk - startclk)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", cpu_time_used);
    return 0;
}
