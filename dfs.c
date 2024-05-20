/** EL2208 Praktikum Pemecahan Masalah dengan C 2022/2023
 *   Modul               : 9 - Tugas Besar
 *   Hari dan Tanggal    : Senin, 20 Mei 2024
 *   Nama (NIM)          : Roger Supriyanto (13222099)
 *   Nama File           : dfs.c
 *   Deskripsi           : mencari jalur dari input file txt dengan metode DFS
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

bool visited[100][100];
char direction[] = "UDLR";

typedef struct {
    int row;
    int col;
} Coordinate;

typedef struct {
    char** cells;
    int rows;
    int cols;
    Coordinate start;
    Coordinate end;
} Maze;

bool isValid(int x, int y, int rows, int cols) {
    return (x >= 0 && x < rows && y >= 0 && y < cols);
}

void display_maze(Maze* maze) {
    for (int i = 0; i < maze->rows; i++) {
        for (int j = 0; j < maze->cols; j++) {
            printf("%c", maze->cells[i][j]);
        }
        printf("\n");
    }
}

void dfs(Maze* maze, Coordinate coordinate, char currentPath[], int pathLength, char ans[][100], int* count) {
    if (coordinate.row == maze->end.row && coordinate.col == maze->end.col) {
        currentPath[pathLength] = '\0';
        strcpy(ans[*count], currentPath);
        (*count)++;
        return;
    }

    visited[coordinate.row][coordinate.col] = true;

    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    for (int i = 0; i < 4; i++) {
        int newX = coordinate.row + dx[i];
        int newY = coordinate.col + dy[i];
        if (isValid(newX, newY, maze->rows, maze->cols) && !visited[newX][newY] && maze->cells[newX][newY] != '#') {
            currentPath[pathLength] = direction[i];
            currentPath[pathLength + 1] = '\0';

            Coordinate newCoordinate = {newX, newY};
            dfs(maze, newCoordinate, currentPath, pathLength + 1, ans, count);

            // Reset currentPath to the state before the recursive call
            currentPath[pathLength] = '\0';
        }
    }

    visited[coordinate.row][coordinate.col] = false;
}

void findPath(Maze* maze) {
    char currentPath[100];
    char ans[100][100];
    int count = 0;
    memset(currentPath, 0, sizeof(currentPath));
    memset(visited, false, sizeof(visited));

    dfs(maze, maze->start, currentPath, 0, ans, &count);

    if (count == 0) {
        printf("-1\n");
    } else {
        for (int i = 0; i < count; i++) {
            printf("Path %d : \n", i + 1);
            printf("(%d,%d)", maze->start.row, maze->start.col);
            int tempx = maze->start.row;
            int tempy = maze->start.col;
            for (int j = 0; j < strlen(ans[i]); j++) {
                if (ans[i][j] == 'D') {
                    tempx++;
                } else if (ans[i][j] == 'U') {
                    tempx--;
                } else if (ans[i][j] == 'R') {
                    tempy++;
                } else if (ans[i][j] == 'L') {
                    tempy--;
                }
                printf(" -> (%d,%d)", tempx, tempy);
            }
            printf("\n");
            printf("Distance: %d\n", (int)strlen(ans[i]));
        }
    }
    printf("Total number of unique paths from 'S' to 'E': %d\n", count);
    int minlen = 999999; // initialize with high value 
    int maxlen = 0; // initialize with low value
    int idmin = -1; 
    int idmax = -1;
    for (int i = 0; i < count; i++) { // iterate to find max and min values with index
        if (strlen(ans[i]) > maxlen) {
            maxlen = strlen(ans[i]);
            idmax = i;
        }
        if (strlen(ans[i]) < minlen) {
            minlen = strlen(ans[i]);
            idmin = i;
        }
    }
    printf("Shortest path distance: %d\n", minlen); // print out the shortest path with disatnce
    printf("Path : (%d, %d)", maze->start.col, maze->start.row);
    int tempx = maze->start.row;
    int tempy = maze->start.col;
    for(int i = 0; i < minlen; i++) {
        if (ans[idmin][i] == 'D') {
            tempx++;
        } else if (ans[idmin][i] == 'U') {
            tempx--;
        } else if (ans[idmin][i] == 'R') {
            tempy++;
        } else if (ans[idmin][i] == 'L') {
            tempy--;
        }
        printf(" -> (%d,%d)", tempx, tempy);
    }
    printf("\n");
    printf("Longest path distance: %d\n", maxlen); // print out longest path with distance
    printf("Path : (%d, %d)", maze->start.col, maze->start.row);
    tempx = maze->start.row;
    tempy = maze->start.col;
    for(int i = 0; i < maxlen; i++) {
        if (ans[idmax][i] == 'D') {
            tempx++;
        } else if (ans[idmax][i] == 'U') {
            tempx--;
        } else if (ans[idmax][i] == 'R') {
            tempy++;
        } else if (ans[idmax][i] == 'L') {
            tempy--;
        }
        printf(" -> (%d,%d)", tempx, tempy);
    }
    printf("\n");
}
int main() {
    Maze maze;
    char filename[256];
    printf("Masukkan nama file: ");
    scanf("%s", filename);
    FILE* file = fopen(filename, "r");
    char line[1024];
    clock_t startclk, endclk; // to check and analyze for time complexity
    double cpu_time_used;

    if (file == NULL) {
        printf("File maze.txt tidak ditemukan\n");
        return 1;
    }
    fgets(line,sizeof(line),file);
    int hitungKolom= strlen(line)-1;
    int hitungBaris = 0;
    rewind(file);
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline character
        hitungBaris++;
        if (hitungKolom != strlen(line)) {
            printf("Found uneven row of line at %d\n", hitungBaris);
            exit(1);
        }
    }
    
    maze.rows = hitungBaris;
    maze.cols = hitungKolom;

    maze.cells = (char**)malloc(maze.rows * sizeof(char*));
    if (maze.cells == NULL) {
        printf("Alokasi memori gagal\n");
        return 1;
    }
    for (int i = 0; i < maze.rows; i++) {
        maze.cells[i] = (char*)malloc((maze.cols + 1) * sizeof(char));
        if (maze.cells[i] == NULL) {
            printf("Alokasi memori gagal\n");
            return 1;
        }
    }

    rewind(file);

    for (int i = 0; i < maze.rows; i++) {
        fgets(maze.cells[i], maze.cols + 1, file);
        fgets(line, sizeof(line), file); // Skip the remaining part of the line if it's longer than cols

        for (int j = 0; j < maze.cols; j++) {
            if (maze.cells[i][j] == 'S') {
                maze.start.row = i;
                maze.start.col = j;
            } else if (maze.cells[i][j] == 'E') {
                maze.end.row = i;
                maze.end.col = j;
            }
        }
    }
    fclose(file);

    printf("Maze layout:\n");
    display_maze(&maze);
    printf("\nStarting path finding...\n");
    startclk = clock();
    findPath(&maze);
    endclk = clock();
    cpu_time_used = ((double) (endclk - startclk)) / CLOCKS_PER_SEC;
    printf("Waktu yang diperlukan: %f",cpu_time_used);
    for (int i = 0; i < maze.rows; i++) {
        free(maze.cells[i]);
    }
    free(maze.cells);

    return 0;
}
