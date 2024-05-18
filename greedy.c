#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h> // for sleep function

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

        system("clear");
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                printf("%c ", maze[i][j]);
            }
            printf("\n");
        }
        printf("\n");
        sleep(1);
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
    // input filename from user
    printf("Enter file name: ");
    scanf("%s", filename);

    FILE *stream = fopen(filename, "r");
    if (stream == NULL) {
        printf("File not found!\n");
        return 1;
    }

    int rows = 0;
    int cols = 0;
    char line[MAX];
    char maze[MAX][MAX];

    while (fgets(line, sizeof(line), stream)) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline character
        strcpy(maze[rows], line);
        rows++;
        cols = strlen(line); // Assuming all rows are of same length
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

    solveMaze(maze, rows, cols, start, end);

    return 0;
}
// Problem found when tracing counter-intuitive paths (differing from heuristic) 
// Ketika sampai di dead-end akan infinite loop
// Code after debug : 
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h> // for sleep function
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

    int dx[] = {-1, 1, 0, 0}; // Changes in x coordinate for each direction
    int dy[] = {0, 0, -1, 1}; // Changes in y coordinate for each direction

    while (current.x != end.x || current.y != end.y) {
        path[pathLength++] = current;

        // Check if the current position is next to the end point
        if ((abs(current.x - end.x) == 1 && current.y == end.y) || (abs(current.y - end.y) == 1 && current.x == end.x)) {
            current.x = end.x;
            current.y = end.y;
        } else {
            // Check if there are any valid neighboring cells
            int foundNextMove = 0;

            // Try moving in each direction
            for (int i = 0; i < 4; i++) {
                int newX = current.x + dx[i];
                int newY = current.y + dy[i];

                if (isValid(newX, newY, rows, cols) && maze[newX][newY] == '.') {
                    current.x = newX;
                    current.y = newY;
                    maze[current.x][current.y] = '*';
                    foundNextMove = 1;
                    break;
                }
            }

            // If no valid moves found, backtrack
            if (!foundNextMove) {
                pathLength--;
                if (pathLength < 0) {
                    printf("No solution found!\n");
                    return;
                }
                current = path[pathLength];
            }
        }
    }

    // Print the shortest path
    printf("Shortest Path:\n");
    printf("(%d, %d)", path[0].y, path[0].x);
    for (int i = 1; i < pathLength; i++) {
        printf(" -> (%d, %d)", path[i].y, path[i].x);
    }
    printf("\nDistance: %d", pathLength);
    printf("\n");
}



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

    int rows = 0;
    int cols = 0;
    char line[MAX];
    char maze[MAX][MAX];
    clock_t startclk, endclk;
    double cpu_time_used;

    while (fgets(line, sizeof(line), stream)) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline character
        strcpy(maze[rows], line);
        rows++;
        cols = strlen(line); // Assuming all rows are of same length
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
    printf("Waktu yang diperlukan: %f",cpu_time_used);
    return 0;
}
*/
